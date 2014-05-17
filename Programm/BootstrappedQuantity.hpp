// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include <string>
#include <utility>
#include <vector>
#include <mutex>

/**
  A quantity that is calculated multiple times using bootstrapped data sets.

  This stores all the individual values and offers mean and standard deviation,
  implicitly assuming that they are normal distributed.
  */
class BootstrappedQuantity {
    public:
        /**
          Appends a new value to the list.

          The function uses a mutex internally is should be thread-safe.
          */
        void append(double point);

        double mean();

        /**
          Calculates standard deviation of the saved data.

          Implementation is taken from http://stackoverflow.com/a/7616783.
          */
        double stddev();

        /**
          Prints a representation of the value and error.
          */
        std::string format();

    protected:
        std::vector<double> data;

        std::mutex append_mutex;
};
