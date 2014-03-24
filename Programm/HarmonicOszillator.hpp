// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef HARMONICOSZILLATOR_H
#define HARMONICOSZILLATOR_H

#include "System.hpp"

#include <vector>

class HarmonicOszillator : public System {
    public:
        virtual double action_step(double cur, double next);
};

#endif /* end of include guard: HARMONICOSZILLATOR_H */
