// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

#ifndef LISTQUANTITY_H
#define LISTQUANTITY_H

#include "Histogram.hpp"

#include <vector>

/**
  Wrapper for lists.

  During the simulation, there are certain quantities that should be kept track
  of. The individual values can be stored in the \ref list. Besides pretty
  printing methods, a snapshot can be taken into an existing Histogram.
  */
class ListQuantity {
    public:
        /**
          Generates a new list.

          @param sites Number of entries to store.
          */
        ListQuantity(size_t sites);

        /**
          Stores the actual data.
          */
        std::vector<double> list;

        /**
          Appends the current trajectory to the histogram.
          */
        void binning_snapshot(Histogram &histogram);

        /**
          Fills the trajectory with purely random numbers.

          @param bound Upper and lower bound for random numbers.
          */
        void set_to_random(double bound);

        /**
          Prints the current trajectory to standard output.
          */
        void print();

        /**
          Save the current trajectory into a text file for plotting.

          @param filename Name of the output file
          */
        void save_plot_file(std::string filename);

    private:
        std::mt19937 mt_engine;
};

#endif /* end of include guard: LISTQUANTITY_H */
