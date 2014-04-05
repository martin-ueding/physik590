// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

#include "BoostHistogram.hpp"
#include "VectorHistogram.hpp"

#include <random>

int main(int argc, char **argv) {
    BoostHistogram hist {100, 10000};

    std::uniform_real_distribution<double> zero_one_dist {0, 1};
    std::mt19937 engine;

    for (unsigned int i {0}; i < 100*1000*1000; i++) {
        hist.push(zero_one_dist(engine));
    }

    hist.save("out/histogram-test.csv");

    return 0;
}
