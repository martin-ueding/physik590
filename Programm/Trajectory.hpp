// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "System.hpp"

#include <vector>

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
        void iteration();

        /**
          Prints the current trajectory to standard output.
          */
        void print();

        /**
          Fills the trajectory with purely random numbers.

          @param bound Upper and lower bound for random numbers.
          */
        void set_to_random(double bound);

        /**
          Gives the action of the system for the current trajectory.
          */
        double action();

    private:
        /**
          System to work with.
          */
        System &system;

        /**
          Positions @f$ \{ x_j \} @f$.
          */
        std::vector<double> x;
};

#endif /* end of include guard: TRAJECTORY_H */
