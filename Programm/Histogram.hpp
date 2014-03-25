// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/density.hpp>
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

        void print() {
            auto hist = density(acc);
            for (unsigned int i = 0; i < hist.size(); i++) {
                std::cout << hist[i].first << "\t" << hist[i].second << std::endl;
            }
        }

        accumulator_set<double, features<tag::density>> acc;
};

#endif /* end of include guard: HISTOGRAM_H */
