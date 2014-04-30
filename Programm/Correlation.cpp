// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Correlation.hpp"

#include <sstream>

Correlation::Correlation(unsigned int distance) : distance {distance} {
}

void Correlation::add_sample(BootstrapSample &sample) {
    size_t count {0};
    double value_0 {0.0};

    for (size_t trajectory_id {0}; trajectory_id < sample.size(); trajectory_id++) {
        ListQuantity &cur = sample[trajectory_id];

        assert(distance + 1 < cur.list.size());

        value_0 += cur.list[0] * cur.list[distance];
        count++;
    }

    append(value_0 / count);
}

std::string Correlation::get_name() {
    std::ostringstream oss;
    oss << "Correlation " << distance;
    return oss.str();
}
