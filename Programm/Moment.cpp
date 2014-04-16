// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Moment.hpp"

#include <iostream>
#include <sstream>

Moment::Moment(int power) : power(power) {
}

void Moment::add_sample(BootstrapSample &sample) {
        size_t count {0};
        double value {0.0};

    for (size_t trajectory_id {0}; trajectory_id < sample.size(); trajectory_id++) {
        ListQuantity &cur = sample[trajectory_id];
        for (double x : cur.list) {
    value += std::pow(x, power);
    count++;
        }
    }

    append(value / count);
}

std::string Moment::get_name() {
    if (power == 1) {
        return "Mean";
    }
    else if (power == 2) {
        return "Variance";
    }
    else {
        std::ostringstream oss;
        oss << "Moment Order " << power;
        return oss.str();
    }
}
