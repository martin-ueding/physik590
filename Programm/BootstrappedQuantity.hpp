// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include <string>
#include <utility>
#include <vector>

/**
  A quantity that is calculated multiple times using bootstrapped data sets.

  This stores all the individual values and offers mean and standard deviation,
  implicitly assuming that they are normal distributed.
  */
class BootstrappedQuantity {
    public:
        /**
          Appends a new value to the list.
          */
        void append(double point);

        /**
          Calculates mean and standard deviation of the saved data.

          Implementation is taken from http://stackoverflow.com/a/7616783.
          */
        std::pair<double, double> mean_and_stddev();

        /**
          Prints a representation of the value and error.
          */
        std::string format();

    protected:
        std::vector<double> data;
};
