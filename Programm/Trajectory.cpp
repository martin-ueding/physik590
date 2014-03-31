// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Trajectory.hpp"

#include "Periodic.hpp"

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

void Trajectory::iteration(int rounds, double margin) {
    for (unsigned int j = 0; j < x.size(); j++) {
        std::uniform_real_distribution<double> distribution(x[j] - margin, x[j] + margin);

        // Wrap j around to create periodic boundary conditions.
        unsigned int j_plus_one = Periodic::wrap(j + 1, x.size());
        unsigned int j_minus_one = Periodic::wrap(j - 1, x.size());

        for (int round = 0; round < rounds; round++) {
            double new_x = distribution(mt_engine);
            //std::cout << "x'\t" << new_x << std::endl;

            double action_difference = system.action_difference(x[j_minus_one], x[j], new_x, x[j_plus_one]);
            //std::cout << "ΔS \t" << action_difference << std::endl;

            if (accept_action_difference(action_difference)) {
                x[j] = new_x;
            }
        }
    }
}

void Trajectory::print() {
    for (double & x_j : x) {
        std::cout << x_j << std::endl;
    }
}

void Trajectory::binning_snapshot(Histogram &histogram) {
    for (unsigned int i = 1; i < x.size() - 10; i++) {
        histogram.acc(x[i]);
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
    auto generator = std::bind(distribution, mt_engine);

    for (unsigned int i = 1; i < x.size() - 1; i++) {
        x[i] = generator();
    }
}
