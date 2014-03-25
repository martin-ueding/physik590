// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Trajectory.hpp"

#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>

Trajectory::Trajectory(int time_sites, System &s) :
    x(std::vector<double>(time_sites)), system(s),
    zero_one_dist(std::uniform_real_distribution<double>(0, 1)) {
}

bool Trajectory::accept_action_difference(double action_difference) {
    if (action_difference < 0) {
        return true;
    }

    return std::exp(- action_difference) > zero_one_dist(mt_engine);
}

double Trajectory::action() {
    return system.action(x);
}

void Trajectory::print() {
    for (double & x_j : x) {
        std::cout << x_j << std::endl;
    }
}

void Trajectory::save_plot_file(std::string filename) {
    std::ofstream outfile(filename);
    outfile << "# j \t x_j" << std::endl;
    for (unsigned int i = 0; i < x.size(); i++) {
        outfile << i << "\t" << x[i] << std::endl;
    }
    outfile.close();
}

void Trajectory::set_to_random(double bound) {
    std::uniform_real_distribution<double> distribution(-bound, bound);
    std::mt19937 engine;
    auto generator = std::bind(distribution, engine);

    for (double & x_j : x) {
        x_j = generator();
    }
}
