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
class Trajectory {
    public:
        /**
          Constructs a lattice with given sites and will use given System.

          @param time_sites Number of time lattice parts @f$ N @f$
          @param s System to work with
          */
        Trajectory(int time_sites, System &s);

        /**
          Does a single iteration.
          */
        void iteration(int rounds, double margin);

        /**
          Gives the action of the system for the current trajectory.
          */
        double action();

        /**
          Positions @f$ \{ x_j \} @f$.
          */
        ListQuantity x;

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

        std::uniform_real_distribution<double> zero_one_dist;
        std::mt19937 mt_engine;
};

#endif /* end of include guard: TRAJECTORY_H */
