// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapPool.hpp"

#include "Correlation.hpp"
#include "ProgressBar.hpp"

#include <future>

BootstrapPool::BootstrapPool(MetropolisDriver &driver, unsigned iterations,
        unsigned position_hist_bins, std::vector<unsigned> &correlation_ts) {
    // Fill pool with trajectories.
    ProgressBar bar {"Populating bootstrap pool", iterations};
    for (unsigned i {0}; i < iterations; ++i) {
        trajectories.push_back(driver.next());
        bar.update(i);
    }
    bar.close();

    // Compute correlations.
    unsigned correlation_size {5};

    ProgressBar bar_corr {"Computing correlation matrices", trajectories.size()};
    for (unsigned t_id {0}; t_id < trajectories.size(); t_id++) {
        CorrFunc map_even;
        CorrFunc map_odd;
        for (unsigned distance : correlation_ts) {
            std::future<Eigen::MatrixXd> f1 = std::async(std::launch::async, correlation, std::ref(trajectories[t_id]), correlation_size, distance, true);

            std::future<Eigen::MatrixXd> f2 = std::async(std::launch::async, correlation, std::ref(trajectories[t_id]), correlation_size, distance, false);

            std::future<Eigen::MatrixXd> f3 = std::async(std::launch::async, correlation, std::ref(trajectories[t_id]), correlation_size, distance+1, true);

            std::future<Eigen::MatrixXd> f4 = std::async(std::launch::async, correlation, std::ref(trajectories[t_id]), correlation_size, distance+1, false);

            map_even.emplace(distance, f1.get());
            map_odd.emplace(distance, f2.get());

            map_even.emplace(distance+1, f3.get());
            map_odd.emplace(distance+1, f4.get());
        }
        even.push_back(std::move(map_even));
        odd.push_back(std::move(map_odd));

        bar_corr.update(t_id);
    }
    bar_corr.close();

    // Compute histograms.
    ProgressBar bar_hist {"Computing histograms", iterations};
    for (unsigned t_id {0}; t_id < trajectories.size(); t_id++) {
        Histogram h {-5, 5, position_hist_bins};

        for (auto x_j : trajectories[t_id]) {
            h(x_j);
        }

        histograms.push_back(h);

        bar_hist.update(t_id);
    }
    bar_hist.close();
}
