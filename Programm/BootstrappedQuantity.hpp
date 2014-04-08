// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef BOOTSTRAPPEDQUANTITY_H
#define BOOTSTRAPPEDQUANTITY_H

#include <vector>

class BootstrappedQuantity {
    public:
        void append(double point);
        std::pair<double, double> mean_and_stddev();

    private:
        std::vector<double> data;
};

#endif /* end of include guard: BOOTSTRAPPEDQUANTITY_H */
