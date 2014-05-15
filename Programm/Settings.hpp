// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include <string>

/**
  Holds the parameters for a simulation.

  The values listed here are the default values that are used unless they are
  overwritten on the command line.
  */
class Settings {
    public:
        unsigned algorithm_version {1};

        /**
          @f$ N @f$ in [CF80].

          Number of division of the time lattice.
          */
        unsigned time_sites {1000};

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

        double gauss_height {0.0};

        double gauss_width {1.0};

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
        unsigned pre_iterations {50};

        /**
          Just like \ref rounds, but for the equilibrium phase.
          */
        unsigned pre_rounds {5};

        /**
          @f$ N_E @f$ in [CF80].
          */
        unsigned iterations {300};

        /**
          @f$ \bar n @f$ in [CF80].
          */
        unsigned rounds {5};

        /**
          Monte Carlo iterations between measurements.
          */
        unsigned iterations_between {2};

        /**
          Seed for the random generator that generates new positions.
          */
        int position_seed {0};

        /**
          Seed for the random generator that is used to decide whether new
          positions should be accepted.
          */
        int accept_seed {0};

        unsigned bootstrap_samples {1000};

        /**
          Number of bins for the position histogram.
          */
        unsigned position_hist_bins {40};

        /**
          Number of bins for the action histogram.
          */
        unsigned action_hist_bins {100};

        /**
          Generates a filename that contains the most important parameters.

          @param name Desired basename
          @return Prefix-Parameters-Suffix
          */
        std::string generate_filename(std::string name);

        std::string report();

        std::string hash();

    private:
        /**
          Separator for parameters in the file name.
          */
        const std::string separator {
            "-"
        };
};
