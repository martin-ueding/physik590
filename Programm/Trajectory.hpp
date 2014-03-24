// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  \file
  */

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "System.hpp"

#include <vector>

class Trajectory {
    public:
        Trajectory(int time_sites, System system);
        void interation();
        void print();

    private:
        System system;
        std::vector<double> x;
};

#endif /* end of include guard: TRAJECTORY_H */
