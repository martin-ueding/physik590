// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef HISTOGRAM_H
#define HISTOGRAM_H


#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/stats.hpp>

#include <iostream>

using namespace boost::accumulators;

class Histogram {
    public:
        Histogram(int bins);

        accumulator_set<double, features<tag::min, tag::max>> acc;

        void print() {
            std::cout << '(' << min( acc ) << ", " << max( acc ) << ")\n";
        }
};


#endif /* end of include guard: HISTOGRAM_H */
