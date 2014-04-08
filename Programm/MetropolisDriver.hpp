// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef METROPOLISDRIVER_H
#define METROPOLISDRIVER_H

#include "BoostHistogram.hpp"
#include "HarmonicOscillator.hpp"
#include "ListQuantity.hpp"
#include "MetropolisAlgorithm.hpp"
#include "Settings.hpp"
#include "VectorHistogram.hpp"

class MetropolisDriver {
    public:
        MetropolisDriver(Settings &settings);

        void run();

    protected:
        Settings &settings;
        System &ho;
        ListQuantity &trajectory;
        MetropolisAlgorithm &ma;

        /**
          Initializes the trajectory to random.

          @param[in] settings Command line options
          @param[in,out] trajectory List with @f$ x @f$ values
          */
        void do_init();
        void do_pre_iterations();
        void do_iterations();
};

#endif /* end of include guard: METROPOLISDRIVER_H */
