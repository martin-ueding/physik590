// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapPool.hpp"

#include "Correlation.hpp"
#include "ProgressBar.hpp"

BootstrapPool::BootstrapPool(MetropolisDriver &driver, Settings &settings) {
    // Fill pool with trajectories.
    ProgressBar bar {"Populating bootstrap pool", settings.iterations};
    for (unsigned i {0}; i < settings.iterations; ++i) {
        trajectories.push_back(driver.next());
        bar.update(i);
    }
    bar.close();

    ProgressBar bar_corr {"Computing correlation matrices", trajectories.size()};
    for (unsigned t_id {0}; t_id < trajectories.size(); t_id++) {
        CorrFunc map_even;
        CorrFunc map_odd;
        for (unsigned distance : settings.correlation_ts) {
            map_even.emplace(distance, correlation(trajectories[t_id], settings, distance, true));
            map_odd.emplace(distance, correlation(trajectories[t_id], settings, distance, false));

            map_even.emplace(distance+1, correlation(trajectories[t_id], settings, distance+1, true));
            map_odd.emplace(distance+1, correlation(trajectories[t_id], settings, distance+1, false));
        }
        even.push_back(std::move(map_even));
        odd.push_back(std::move(map_odd));

        bar_corr.update(t_id);
    }
    bar_corr.close();

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
