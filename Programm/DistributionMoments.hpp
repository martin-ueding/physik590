// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef DISTRIBUTIONMOMENTS_H
#define DISTRIBUTIONMOMENTS_H

#include <cmath>
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

        /**
          Gives the standard deviation @f$ \sigma @f$.

          This assumes that the distribution is a normal distribution where @f$
          \sigma = \sqrt{\langle x^2 \rangle} @f$ holds. That assumtion is not
          checked by this function.
          */
        double sigma();

        static constexpr double pi {4 * std::atan(1)};
        static constexpr double sigma_theory {1 / (0.59 * std::sqrt(2 * pi))};

    protected:
        std::vector<double> moments;
};

#endif /* end of include guard: DISTRIBUTIONMOMENTS_H */
