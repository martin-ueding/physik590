// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrappedQuantity.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>

void BootstrappedQuantity::append(double point) {
    if (std::isnan(point)) {
        nans++;
        return;
    }
    std::unique_lock<std::mutex> lock {append_mutex};
    data.push_back(point);
}

double BootstrappedQuantity::mean() {
    if (data.size() == 0) {
        std::ostringstream oss;
        oss << "No elements to calculate mean from";
        if (nans > 0) {
            oss << ", but " << nans << " NaNs.";
        }
        else {
            oss << ".";
        }
        throw std::runtime_error{oss.str()};
    }
    double sum {std::accumulate(data.begin(), data.end(), 0.0)};
    double mean {sum / data.size()};

    return mean;
}

double BootstrappedQuantity::stddev() {
    if (data.size() == 0) {
        throw std::runtime_error{"No elements to calculate standard deviation from!"};
    }

    std::vector<double> diff(data.size());
    std::transform(data.begin(), data.end(), diff.begin(), std::bind2nd(std::minus<double>(), mean()));
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stddev = std::sqrt(sq_sum / data.size());

    return stddev;
}

std::string BootstrappedQuantity::format() {
    std::ostringstream oss;
    oss << mean() << " ± " << stddev();
    return oss.str();
}
