// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Moment.hpp"

Moment::Moment(size_t moment_count) :
    moments(std::vector<double>(moment_count + 1)) {
}

void Moment::push(const double val) {
    for (unsigned int i {0}; i < moments.size(); i++) {
        moments[i] += std::pow(val, i);
    }
}

double Moment::operator[](int i) {
    return moments[i] / moments[0];
}

size_t Moment::size() {
    return moments.size();
}

double Moment::sigma() {
    double sigma {std::sqrt(this->operator[](2))};

    return sigma;
}
