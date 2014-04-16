// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef BOOSTHISTOGRAM_H
#define BOOSTHISTOGRAM_H

#include "Histogram.hpp"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/density.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/stats.hpp>

#include <string>

/**
  Wrapper for Boost histogram.
  */
class BoostHistogram : public Histogram {
    public:
        /**
          New Histogram.

          The Boost histogram will take @c cache amounts of data points before
          it starts with the binning. Based on this number of elements, the
          minimum and maximum are set. The bins are spaced evenly between.

          @param bins Number of bins.
          @param cache Cache entries
          */
        BoostHistogram(int bins, size_t cache);

        virtual void write_histogram(std::ostream &outfile);

        virtual void push(double value);

        /**
          Boost accumulator.

          This stores the data, add your data here.
          */
        boost::accumulators::accumulator_set <
        double,
        boost::accumulators::features <
        boost::accumulators::tag::min,
        boost::accumulators::tag::max,
        boost::accumulators::tag::mean,
        boost::accumulators::tag::density
        >
        >
        acc;
};

#endif /* end of include guard: BOOSTHISTOGRAM_H */
