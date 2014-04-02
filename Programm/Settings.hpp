// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef SETTINGS_H
#define SETTINGS_H

#include <cstddef>

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
        bool fix_zeroth_coordinate;

        int position_hist_bins;
        int action_hist_bins;
};

#endif /* end of include guard: SETTINGS_H */
