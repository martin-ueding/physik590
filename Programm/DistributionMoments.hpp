// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef DISTRIBUTIONMOMENTS_H
#define DISTRIBUTIONMOMENTS_H

#include <cstddef>
#include <vector>

class DistributionMoments {
    public:
        /**
          Creates a new moment calculator.

          @param[in] moment_count Maximum order of moments to calculate.
          */
        DistributionMoments(size_t moment_count);

        /**
          Add a new value, incremental calculation.

          @param[in] val New value
          */
        void push(const double val);

        double operator[](int i);

        size_t size();

    protected:
        std::vector<double> moments;
};

#endif /* end of include guard: DISTRIBUTIONMOMENTS_H */
