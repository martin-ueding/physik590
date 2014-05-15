// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapPool.hpp"

#include "ProgressBar.hpp"

BootstrapPool::BootstrapPool(MetropolisDriver &driver, unsigned iterations) {
    // Fill pool with trajectories.
    ProgressBar bar {"Populating bootstrap pool", iterations};
    for (unsigned i {0}; i < iterations; ++i) {
        trajectories.push_back(driver.next());
        bar.update(i);
    }
    bar.close();

    // Compute correlations.
    unsigned correlation_size {10};

    ProgressBar bar {"Computing correlation matrices", trajectories.size()};
    for (unsigned t_id {0}; t_id < trajectories.size(); t_id++) {
        std::map<unsigned, boost::numeric::ublas::matrix> map_even;
        std::map<unsigned, boost::numeric::ublas::matrix> map_odd;
        for (unsigned distance = 0; distance < settings.time_sites / 2; distance += distance / 5 + 1) {
            map_even.insert(map.value_type {distance, Correlation.corrleation(trajectories[t_id], correlation_size, distance, true)});
            map_odd.insert(map.value_type {distance, Correlation.corrleation(trajectories[t_id], correlation_size, distance, false)});
        }
        even.append(std::move(map_even));
        odd.append(std::move(map_odd));

        bar.update(t_id);
    }
    bar.close();

    // Compute histograms.
    ProgressBar bar {"Computing histograms", iterations};
    for (unsigned t_id {0}; t_id < trajectories.size(); t_id++) {
        Histogram h {-5, 5, settings.position_hist_bins};

        for (auto x_j : trajectories[t_id]) {
            h(x_j);
        }

        histograms.push_back(h);

        bar.update(t_id);
    }
    bar.close();
}
