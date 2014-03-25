// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

#ifndef HARMONICOSZILLATOR_H
#define HARMONICOSZILLATOR_H

#include "System.hpp"

#include <vector>

/**
  Harmonic oscillator.
  */
class HarmonicOszillator : public System {
    public:
        HarmonicOszillator(double time_step, double mass, double omega_squared);

        virtual double potential(double x);

    private:
        /**
          Sping constant @f$ k @f$ or @f$ \omega^2 @f$.
          */
        double omega_squared;
};

#endif /* end of include guard: HARMONICOSZILLATOR_H */
