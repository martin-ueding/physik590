// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Trajectory.hpp"

#include <functional>
#include <iostream>
#include <random>

Trajectory::Trajectory(int time_sites, System system) : x(std::vector<double>(time_sites)) {
}

void Trajectory::print() {
    for (double &x_j : x) {
        std::cout << x_j << std::endl;
    }
}

void Trajectory::set_to_random(double bound) {
    std::uniform_real_distribution<double> distribution(-bound, bound);
    std::mt19937 engine;
    auto generator = std::bind(distribution, engine);

    for (double &x_j : x) {
        x_j = generator();
    }
}
