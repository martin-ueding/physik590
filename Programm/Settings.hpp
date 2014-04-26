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

          Number of division of the time lattice.
          */
        size_t time_sites {1000};

        /**
          @f$ M_0 @f$ in [CF80].

          Mass of the particle.
          */
        double mass {1};

        /**
          @f$ a @f$ in [CF80].

          Step size of the time lattice.
          */
        double time_step {0.1};

        /**
          @f$ \mu^2 @f$ in [CF80].

          “Spring constant”.
          */
        double mu_squared {1};

        /**
          Spread of positions in the random initialization.
          */
        double initial_random_width {0.632456};

        /**
          @f$ \Delta @f$ in [CF80].
          */
        double margin {0.632456};

        /**
          @f$ N_t @f$ in [CF80].
          */
        int pre_iterations {50};

        /**
          Just like \ref rounds, but for the equilibrium phase.
          */
        int pre_rounds {5};

        /**
          @f$ N_E @f$ in [CF80].
          */
        size_t iterations {300};

        /**
          @f$ \bar n @f$ in [CF80].
          */
        int rounds {5};

        /**
          Monte Carlo iterations between measurements.
          */
        int iterations_between {2};

        /**
          Seed for the random generator that generates new positions.
          */
        int position_seed {0};

        /**
          Seed for the random generator that is used to decide whether new
          positions should be accepted.
          */
        int accept_seed {0};

        /**
          Number of bins for the position histogram.
          */
        size_t position_hist_bins {40};

        /**
          Number of bins for the action histogram.
          */
        size_t action_hist_bins {100};

        /**
          Generates a filename that contains the most important parameters.

          @param[in] prefix Prefix.
          @param[in] suffix Suffix.
          @return Prefix-Parameters-Suffix.
          */
        std::string generate_filename(std::string prefix, std::string suffix);

    private:
        /**
          Separator for parameters in the file name.
          */
        const std::string separator {
            "-"
        };
};

#endif /* end of include guard: SETTINGS_H */
