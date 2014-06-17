// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "Oscillator.hpp"

#include <random>
#include <string>

/**
  Class that holds the Metropolis algorithm.
  */
class MetropolisAlgorithm {
    public:
        /**
          Constructs a lattice with given sites and will use given System.

          To change the initial seeds of the random generators, specify a
          value.

          @param s System to work with
          @param position_seed Seed for position generator
          @param accept_seed Seed for accept generator
          */
        MetropolisAlgorithm(Oscillator &s, int position_seed, int accept_seed);

        /**
          Does a single iteration.

          @param x Trajectory of the particle
          */
        void iteration(std::vector<double> &x, int rounds, double margin);

        double get_accept_rate();
        double get_accept_rate_negative();
        double get_accept_rate_exponential();

        void reset_accept_rate();

        void re_seed(int seed);

    private:
        /**
          System to work with.
          */
        Oscillator &system;

        /**
          Decides whether a new action should be accepted.

          The Metropolis algorithm will accept it, if @f$ \Delta s < 0 @f$. In
          case @f$ \Delta S \geq 0 @f$, a random number @f$ r @f% is taken from
          0 to one. If @f$ \exp(-S) \geq r @f$ holds, the new action is
          accepted.

          @param action_difference @f$ \Delta s @f$
          @returns Is accepted
          */
        bool accept_action_difference(double action_difference);

        std::uniform_real_distribution<double> zero_one_dist {0, 1};
        std::mt19937 position_engine;
        std::mt19937 accept_engine;

        unsigned samples {0};
        unsigned accepted_negative {0};
        unsigned accepted_exponential {0};
};
