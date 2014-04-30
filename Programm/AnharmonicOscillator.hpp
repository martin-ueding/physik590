// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "System.hpp"

#include <vector>

/**
  Harmonic oscillator.
  */
class AnharmonicOscillator : public System {
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

        virtual double potential(double x);

    protected:
        /**
          Sping constant @f$ k @f$ or @f$ \mu^2 @f$.
          */
        double mu_squared;

        double gauss_height;
        double gauss_width;
};
