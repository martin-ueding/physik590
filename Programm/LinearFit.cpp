// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "LinearFit.hpp"

#include <gsl_fit.h>

LinearFit:: LinearFit(std::vector<double> x, std::vector<double> y,
                      std::vector<double> y_err) {
    /*
    int gsl_fit_wlinear(const double *x, const size_t xstride, const double *w,
                        const size_t wstride, const double *y,
                        const size_t ystride, size_t n, double *c0, double *c1,
                        double *cov00, double *cov01, double * cov11,
                        double *chisq)
    */
    // TODO Assert that x, y and y_err have the same size.

    // Invert errors to give weights.
    std::vector<double> weights(y_err.size());
    for (size_t i {0}; i != y.size(); ++y) {
        weights[i] = 1 / y_err[i];
    }

    gsl_fit_wlinear(x.data(), 1, weights.data(), 1, , y.data(), 1, x.size(),
                    &c0, &c1, &cov00, &cov01, &cov11, &chisq);
}
