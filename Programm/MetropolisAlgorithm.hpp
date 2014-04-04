// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "System.hpp"
#include "ListQuantity.hpp"

#include <random>
#include <string>

/**
  Class that holds the Metropolis algorithm.
  */
class MetropolisAlgorithm {
    public:
        /**
          Constructs a lattice with given sites and will use given System.

          @param trajectory Trajectory of the particle
          @param s System to work with
          */
        MetropolisAlgorithm(ListQuantity &trajectory, System &s);

        /**
          Does a single iteration.
          */
        void iteration(int rounds, double margin);

        /**
          Positions @f$ \{ x_j \} @f$.
          */
        ListQuantity &x;

    private:
        /**
          System to work with.
          */
        System &system;

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
};

#endif /* end of include guard: TRAJECTORY_H */
