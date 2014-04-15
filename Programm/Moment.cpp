// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Moment.hpp"

Moment::Moment(int power) : power(power) {
}

void Moment::push(const double new_val) {
    value += std::pow(new_val, power);
    count++;
}

void Moment::add_sample(BootstrapSample &sample) {
    for (auto trajectory : sample.trajectories) {
        for (auto x : trajectory->list) {
            push(x);
        }
    }
}
