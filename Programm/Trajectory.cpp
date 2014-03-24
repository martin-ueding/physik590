// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Trajectory.hpp"

#include <iostream>

Trajectory::Trajectory(int time_sites, System system) : x(std::vector<double>(time_sites)) {

}

void Trajectory::print() {
    for (auto &x_j : x) {
        std::cout << x_j << std::endl;
    }
}
