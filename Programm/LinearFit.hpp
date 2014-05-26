// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include <vector>

class LinearFit {
    public:
        LinearFit(std::vector<double> x, std::vector<double> y);
        LinearFit(std::vector<double> x, std::vector<double> y,
                  std::vector<double> y_err);

        double c0;
        double c1;
        double cov00;
        double cov01;
        double cov11;
        double chisq;
};
