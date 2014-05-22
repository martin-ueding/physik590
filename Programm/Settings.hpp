// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include <string>
#include <vector>

/**
  Holds the parameters for a simulation.

  The values listed here are the default values that are used unless they are
  overwritten on the command line.
  */
class Settings {
    public:
        /**
          Generates a filename that contains the most important parameters.

          @param name Desired basename
          @return Prefix-Parameters-Suffix
          */
        std::string generate_filename(std::string name);

        std::string report();

        std::string hash();

        /**
          Gives the number of eigenvalues that can be computed.
          */
        unsigned max_energyvalue();

        /**
          Convertes an eigenvalue index into a energy index.

          The first even eigenvalues are 2, 4, 6. Those have indices 0, 1 and
          2. The first odd eigenvalues are 1, 3, 5. Those have the same
          indices. The method therefore is to multipy the index with two and
          add one if it is odd.

              2 4 6 8 n
              0 1 2 3 index

              1 3 5 7 n
              0 1 2 3 index

          @param[in] n n
          @return index
          */
        unsigned matrix_to_state(unsigned i, bool even);

        unsigned state_to_matrix(unsigned n);

        /**
          Calculates options based on the other ones.

          Call this whenever you have changed any values.
          */
        void compute();

        unsigned algorithm_version {3};

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

        /**
          Inverse scattering length \f$ 1/a_0 \f$.
          */
        double inverse_scattering_length {0.0};

        /**
          Standard deviation for the gauss curve that approximates the delta
          distribution.
          */
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

        double position_hist_min {-5};
        double position_hist_max {-5};

        /**
          Number of bins for the action histogram.
          */
        unsigned action_hist_bins {100};

        /**
         Time points to calculate correlations for.

         The correlation matrix is a function of \f$ t \f$ and \f$ t_0 \f$,
         like \f$ C_{ij} (t, t_0) \f$. The value of \f$ t_0 \f$ can be fixed or
         half of \f$ t \f$. Either way, \f$ t_0 \f$ will be calculated from \f$
         t \f$.

         Since the slope of the eigenvalues is needed, for each given value,
         the correlation for the next step has to be calculated as well. This
         array only contains the first one.
         */
        std::vector<unsigned> correlation_ts;

        /**
          Size of the correlation matrices.
          */
        unsigned correlation_size {4};

        unsigned export_potential_steps {1000};
        double export_potential_bound {5.0};

        double corr_tau_max = 4;

    private:
        /**
          Separator for parameters in the file name.
          */
        const std::string separator {
            "-"
        };
};
