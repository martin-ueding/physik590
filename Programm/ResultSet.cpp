// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "ResultSet.hpp"
#include "Settings.hpp"

#include <iostream>
#include <random>
#include <thread>

typedef std::pair<const unsigned int, std::shared_ptr<Correlation>> correlation_pair;

ResultSet::ResultSet(BootstrapPool &pool, Settings &settings) :
    dens {PositionDensity{ -5, 5, settings.position_hist_bins}},
     pool(pool),
     bootstrap_sample_count {settings.bootstrap_samples},
     settings {settings},
bar {ProgressBar{"Creating samples", settings.bootstrap_samples}} {

    for (unsigned i = 0; i < settings.time_sites / 2; i += i / 5 + 1) {
        add_correlation(i);
    }

    compute_using_pool();
}

void ResultSet::add_correlation(unsigned int distance) {
    std::shared_ptr<Correlation> next_correlation { new Correlation {distance}};
    correlations.insert(correlation_pair {
        distance,
        std::shared_ptr<Correlation>{next_correlation}
    });

    computables.push_back(next_correlation);
}

void ResultSet::save_correlations(std::string outfilename) {
    std::ofstream handle {outfilename};
    for (auto & pair : correlations) {
        auto m_and_s = pair.second->mean_and_stddev();
        handle << pair.first << "\t" << m_and_s.first << "\t" << m_and_s.second << std::endl;
    }
    handle.close();
}

void ResultSet::operator()() {
    size_t sample_id;
    while ((sample_id = counter()) < bootstrap_sample_count) {
        bar.update(sample_id);
        BootstrapSample sample {pool, engine};

        for (auto computable : computables) {
            computable->add_sample(sample);
        }

        dens.add_sample(sample);
    }
}

void ResultSet::compute_using_pool() {
    unsigned int cpu_count = std::thread::hardware_concurrency();
    //cpu_count = 1;

    std::vector<std::thread> workers;
    for (unsigned int i = 0; i < cpu_count; i++) {
        workers.emplace_back(std::ref(*this));
    }

    for (auto & worker : workers) {
        worker.join();
    }

    bar.close();
}

void ResultSet::print_results() {
    std::ofstream resultfile {settings.generate_filename("results.txt")};
    for (auto computable : computables) {
        resultfile << computable->get_name() << ": " << computable->format() << std::endl;
    }

    std::ofstream of {settings.generate_filename("histogram-position-resultset.csv")};
    of << settings.report();
    dens.write_histogram(of);
}
