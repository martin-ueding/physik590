// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef BOOTSTRAPDRIVER_H
#define BOOTSTRAPDRIVER_H

#include "ResultSet.hpp"
#include "Settings.hpp"

/**
  Runs multiple Monte Carlo simulations and does a meta analysis of the
  results.
  */
class BootstrapDriver {
    public:
        void run(Settings &s);
    protected:
        ResultSet results;
};

#endif /* end of include guard: BOOTSTRAPDRIVER_H */
