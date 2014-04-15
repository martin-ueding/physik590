// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef HARMONICOSZILLATOR_H
#define HARMONICOSZILLATOR_H

#include "System.hpp"

#include <vector>

/**
  Harmonic oscillator.
  */
class HarmonicOscillator : public System {
    public:
        /**
          Creates a new harmonic oscillator.

          The first two parameters are passed off to the System constructor.

          @param time_step Time lattice spacing @f$ a @f$
          @param mass Mass @f$ m @f$
          @param mu_squared Spring constant @f$ \mu^2 @f$
          */
        HarmonicOscillator(double time_step, double mass, double mu_squared);

        virtual double potential(double x);

        virtual double ground_energy(double moment_2);

    private:
        /**
          Sping constant @f$ k @f$ or @f$ \mu^2 @f$.
          */
        double mu_squared;
};

#endif /* end of include guard: HARMONICOSZILLATOR_H */
