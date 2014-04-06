// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

/**
  Holds the parameters for a simulation.

  The values listed here are the default values that are used unless they are
  overwritten on the command line.
  */
class Settings {
    public:
        /**
          @f$ N @f$ in [CF80].
          */
        size_t time_sites {1000};

        /**
          @f$ M_0 @f$ in [CF80].
          */
        double mass {1};

        /**
          @f$ a @f$ in [CF80].
          */
        double time_step {0.1};

        /**
          @f$ \mu^2 @f$ in [CF80].
          */
        double mu_squared {1};

        double initial_random_width {0.63};

        /**
          @f$ \Delta @f$ in [CF80].
          */
        double margin {0.632456};

        /**
          @f$ N_t @f$ in [CF80].
          */
        int pre_iterations {50};
        int pre_rounds {5};

        /**
          @f$ M @f$ in [CF80].
          */
        size_t iterations {10000};
        int rounds {5};

        /**
          @f$ \bar n @f$ in [CF80].
          */
        int iterations_between {2};
        int position_seed {0};
        int accept_seed {0};

        size_t position_hist_bins {1000};
        size_t action_hist_bins {100};

        std::string generate_filename(std::string prefix, std::string suffix);

    private:
        const std::string separator {
            "-"
        };
};

#endif /* end of include guard: SETTINGS_H */
