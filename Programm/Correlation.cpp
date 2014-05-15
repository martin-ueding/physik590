// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Correlation.hpp"

#include "Periodic.hpp"

#include <cassert>
#include <sstream>

Correlation::Correlation(unsigned int distance) : distance {distance} {
}

void Correlation::add_sample(BootstrapSample &sample) {
    size_t count {0};
    double value {0.0};

    for (size_t trajectory_id {0}; trajectory_id < sample.size(); trajectory_id++) {
        value += average_all(sample[trajectory_id].list, distance);
        count++;
    }

    append(value / count);
}

double Correlation::average_all(std::vector<double> &list, const unsigned distance) {
    double sum {0};
    size_t max {list.size()};

    for (unsigned i {0}; i < max; ++i) {
        sum += list[i] * list[Periodic::wrap(i + distance, max)];
    }

    return sum / max;
}

std::string Correlation::get_name() {
    std::ostringstream oss;
    oss << "Correlation " << distance;
    return oss.str();
}
