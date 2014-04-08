// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "DistributionMoments.hpp"

#include <cmath>

DistributionMoments::DistributionMoments(const unsigned int moment_count) :
    moments(std::vector<double>(moments)) {
}

void DistributionMoments::push(const double val) {
    for (unsigned int i {0}; i < moments.size(); i++) {
        moments[i] += std::pow(val, i);
    }
}

double DistributionMoments::operator[](int i) {
    return moments[i] / moments[0];
}
