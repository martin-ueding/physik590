// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrappedQuantity.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>

void BootstrappedQuantity::append(double point) {
    data.push_back(point);
}

std::pair<double, double> BootstrappedQuantity::mean_and_stddev() {
    double sum {std::accumulate(data.begin(), data.end(), 0.0)};
    double mean {sum / data.size()};

    //double squared_sum {std::inner_product(data.begin(), data.end(), data.begin(), 0.0)};
    //double stddev {std::sqrt(squared_sum / data.size() - mean * mean)};

    std::vector<double> diff(data.size());
    std::transform(data.begin(), data.end(), diff.begin(), std::bind2nd(std::minus<double>(), mean));
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stddev = std::sqrt(sq_sum / data.size());

    return std::pair<double, double> {mean, stddev};
}

void BootstrappedQuantity::print() {
    auto ms = mean_and_stddev();
    std::cout << ms.first << " ± " << ms.second << std::endl;
}
