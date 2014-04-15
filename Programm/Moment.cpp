// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Moment.hpp"

#include <iostream>

Moment::Moment(int power) : power(power) {
}

void Moment::push(const double new_val) {
    value += std::pow(new_val, power);
    count++;
}

void Moment::add_sample(BootstrapSample &sample) {
    //std::cout << sample << std::endl;
    //std::cout << sample.trajectories << std::endl;
    std::cout << "add_sample" << std::endl;
    for (Trajectory *trajectory : sample.trajectories) {
        size_t trajectory_list_size {trajectory->list.size()};
        std::cout << trajectory_list_size << std::endl;
        for (double x : trajectory->list) {
            //std::cout << "add_sample: " << x << std::endl;
            push(x);
        }
    }
}
