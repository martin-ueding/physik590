// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>

/**
  Models a one particle system.

  Basically, this will be able to compute the action for a given trajectory.
  All that is needed is an action_step() function that will give an
  integration step of the lagrangian, yielding the action for one time step.
  */
class System {
    public:
        /**
          Computes the total action for the given trajectory.

          It uses action_step() and just sums it all up.

          @param x Trajectory @f$ \{x_j\} @f$
          @returns Action @f$ S @f$
          */
        double action(std::vector<double> &x);

        /**
          Gives the action for the trajectory between two points.

          @param cur Current: @f$ x_{j} @f$
          @param next Next: @f$ x_{j+1} @f$
          @returns Action of single step @f$ S @f$
          */
        virtual double action_step(double cur, double next) = 0;

        /**
          Computes the difference in action when one @f$ x_j @f$ is varied.

          It computes @f$ [S(x_{j-1}, x_j') + S(x_j', x_{j+1})] - [S(x_{j-1},
          x_j) + S(x_j, x_{j+1})] @f$.

          @param prev Previous: @f$ x_{j-1} @f$
          @param cur Current: @f$ x_{j} @f$
          @param alt Alternative: @f$ x_{j}' @f$
          @param next Next: @f$ x_{j+1} @f$
          @returns Difference in action @f$ S @f$
          */
        double action_difference(double prev, double cur, double alt,
                                 double next);

        /**
          Time step @f$ a @f$.
          */
        double a = 1.0;

        /**
          Mass @f$ m @f$.
          */
        double mass = 1.0;
};

#endif /* end of include guard: SYSTEM_H */
