// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class Settings {
    public:
        size_t time_sites;
        double mass;
        double time_step;
        double mu_squared;

        double initial_random_width;
        double margin;
        int pre_iterations;
        int pre_rounds;

        size_t iterations;
        int rounds;
        int iterations_between;
        int position_seed;

        size_t position_hist_bins;
        size_t action_hist_bins;

        std::string generate_filename(std::string prefix, std::string suffix);

        const std::string separator {
            "-"
        };
};

#endif /* end of include guard: SETTINGS_H */
