// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapPool.hpp"

#include "Correlation.hpp"

#include <fstream>
#include <thread>

BootstrapPool::BootstrapPool() {
}

BootstrapPool::BootstrapPool(MetropolisDriver &driver, Settings &settings) {
    // Fill pool with trajectories.
    ProgressBar bar {"Populating bootstrap pool", settings.iterations};
    for (unsigned i {0}; i < settings.iterations; ++i) {
        trajectories.push_back(driver.next());
        bar.update(i);
    }
    bar.close();

#ifndef NDEBUG
    std::cout << "BootstrapPool::trajectories now contains " << trajectories.size() << " items." << std::endl;
#endif

    even.resize(trajectories.size());
    odd.resize(trajectories.size());

    std::vector<std::thread> workers;

    ProgressBar bar_corr {"Computing correlation matrices", static_cast<unsigned>(trajectories.size())};
    for (unsigned i {0}; i < settings.max_cores; i++) {
        workers.emplace_back(std::ref(*this), std::ref(settings), std::ref(bar_corr));
    }
    for (unsigned i {0}; i < settings.max_cores; i++) {
        workers[i].join();
    }
    bar_corr.close();

#ifndef NDEBUG
    std::cout << "BootstrapPool::even now contains " << even.size() << " items." << std::endl;
    std::cout << "BootstrapPool::odd now contains " << odd.size() << " items." << std::endl;
#endif

    // Compute histograms.
    ProgressBar bar_hist {"Computing histograms", settings.iterations};
    for (unsigned t_id {0}; t_id < trajectories.size(); t_id++) {
        Histogram h {settings.position_hist_min, settings.position_hist_max, settings.position_hist_bins};

        for (auto x_j : trajectories[t_id]) {
            h(x_j);
        }

        histograms.push_back(h);

        bar_hist.update(t_id);
    }
    bar_hist.close();
}

void BootstrapPool::operator()(Settings &settings, ProgressBar &bar_corr) {
    unsigned t_id;
    while ((t_id = t_id_atom++) < trajectories.size()) {
        CorrFunc map_even;
        CorrFunc map_odd;
        for (unsigned distance : settings.correlation_ts) {
            map_even.emplace(distance, correlation(trajectories[t_id], settings, distance, true));
            map_odd.emplace(distance, correlation(trajectories[t_id], settings, distance, false));
        }
        std::lock_guard<std::mutex> {mutex};
        even[t_id] = map_even;
        odd[t_id] = map_odd;

        bar_corr.update(t_id);
    }
}

void save_pool(std::shared_ptr<BootstrapPool> pool, Settings &settings) {
    ProgressBar bar {"Serializing", 1};
    std::ofstream ofs {settings.generate_filename("pool.bin")};
    boost::archive::binary_oarchive oa {ofs};
    oa << *pool;
}

void load_into_pool(std::shared_ptr<BootstrapPool> &pool, Settings &settings) {
    ProgressBar bar {"Loading data", 1};
    pool = std::unique_ptr<BootstrapPool> {new BootstrapPool {}};
    std::ifstream ifs(settings.load_filename);
    boost::archive::binary_iarchive ia(ifs);
    ia >> *pool;

#ifndef NDEBUG
    bar.close();

    std::cout << "pool.even.size() " << pool->even.size() << std::endl;
    std::cout << "pool.odd.size() " << pool->odd.size() << std::endl;
    std::cout << "pool.histograms.size() " << pool->histograms.size() << std::endl;
    std::cout << "pool.histograms[0].size() " << pool->histograms[0].size() << std::endl;
    std::cout << "pool.histograms[0].get_min() " << pool->histograms[0].get_min() << std::endl;
    std::cout << "pool.histograms[0].get_max() " << pool->histograms[0].get_max() << std::endl;
    std::cout << "pool.histograms[0][0] " << pool->histograms[0][0] << std::endl;
#endif
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
template<class Archive>
void BootstrapPool::serialize(Archive &ar, const unsigned version) {
            ar &trajectories;
            ar &even;
            ar &odd;
            ar &histograms;
        }
#pragma clang diagnostic pop
