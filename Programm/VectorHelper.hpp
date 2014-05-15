// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include <random>
#include <vector>

/**
  Helper for std::vector<double>.
  */
class VectorHelper {
    public:
        /**
          Fills the vector with purely random numbers.

          @param[in,out] list List of values
          @param[in] bound Upper and lower bound for random numbers.
          */
        void set_to_random(std::vector<double> &list, double bound);

        /**
          Save the current trajectory into a text file for plotting.

          @param[in,out] list List of values
          @param filename Name of the output file
          @param preamble Preamble documenting settings of this run
          */
        void save_plot_file(std::vector<double> &list, std::string filename, std::string preamble);

    private:
        std::mt19937 mt_engine;
};
