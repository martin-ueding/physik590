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
    std::cout << "add_sample()" << std::endl;
    for (size_t trajectory_id {0}; trajectory_id < sample.size(); trajectory_id++) {
        ListQuantity &cur = sample[trajectory_id];
        for (double x : cur.list) {
            push(x);
        }
    }

    append(value / count);
    value = 0;
    count = 0;
}
