// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include <string>
#include <vector>

/**
  Models a one particle system.

  Basically, this will be able to compute the action for a given trajectory.
  All that is needed is an potential() function that will give a potential
  energy.
  */
class Oscillator {
    public:
        /**
          Creates a new anharmonic oscillator.

          The first two parameters are passed off to the System constructor.

          @param time_step Time lattice spacing @f$ a @f$
          @param mass Mass @f$ m @f$
          @param mu_squared Spring constant @f$ \mu^2 @f$
          */
        AnharmonicOscillator(double time_step, double mass, double mu_squared,
                             double gauss_height, double gauss_width);

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
        double action_step(double cur, double next);

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
          Potential @f$ \phi(x) @f$.

          @param x Position @f$ x @f$
          */
        double potential(double x);

        /**
          Exports a X-Y-file for the potential.
          */
        void export_potential(std::string filename, std::string preamble);

        /**
          Time step @f$ a @f$.
          */
        double time_step = 1.0;

        /**
          Mass @f$ m @f$.
          */
        double mass = 1.0;

    protected:
        /**
          Sping constant @f$ k @f$ or @f$ \mu^2 @f$.
          */
        double mu_squared;

        double gauss_height;
        double gauss_width;

    private:
        double gauss_width_squared;
};
