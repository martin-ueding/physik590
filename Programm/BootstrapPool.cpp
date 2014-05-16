// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapPool.hpp"

#include "Correlation.hpp"
#include "ProgressBar.hpp"

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
            map_even.insert(CorrFunc::value_type {distance, Correlation::correlation(trajectories[t_id], correlation_size, distance, true)});
            map_odd.insert(CorrFunc::value_type {distance, Correlation::correlation(trajectories[t_id], correlation_size, distance, false)});

            map_even.insert(CorrFunc::value_type {distance+1, Correlation::correlation(trajectories[t_id], correlation_size, distance+1, true)});
            map_odd.insert(CorrFunc::value_type {distance+1, Correlation::correlation(trajectories[t_id], correlation_size, distance+1, false)});
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
