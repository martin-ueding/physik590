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

#include <string>

using namespace boost::accumulators;

/**
  Wrapper for Boost histogram.
  */
class Histogram {
    public:
        /**
          New Histogram.

          The Boost histogram will take @c cache amounts of data points before
          it starts with the binning. Based on this number of elements, the
          minimum and maximum are set. The bins are spaced evenly between.

          @param bins Number of bins.
          @param cache Cache entries
          */
        Histogram(int bins, int cache);

        /**
          Prints the current histogram contents to the standard output.
          */
        void print();

        /**
          Saves the current histogram to the given file.
          */
        void save(std::string filename);

        /**
          Boost accumulator.

          This stores the data, add your data here.
          */
        accumulator_set<double, features<tag::density>> acc;

        const int MAX_CACHE = 1024 * 1024 * 1024 / sizeof(double);

    private:
        /**
          Writes the current histogram into the given stream.

          @param outfile Stream to write to
          */
        void write_histogram(std::ostream &outfile);
};

#endif /* end of include guard: HISTOGRAM_H */
