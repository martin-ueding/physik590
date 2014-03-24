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
        virtual double action_step(double cur, double next);

        /**
          Potential @f$ \phi(x) @f$.

          @param x Position @f$ x @f$
          */
        virtual double potential(double x) {
            return 0.5 * k * x * x;
        }

    private:
        /**
          Sping constant @f$ k @f$ or @f$ \omega^2 @f$.
          */
        double k = 1.0;
};

#endif /* end of include guard: HARMONICOSZILLATOR_H */
