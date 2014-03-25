// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Histogram.hpp"

Histogram::Histogram(int bins) : acc(accumulator_set<double, features<tag::density>>(tag::density::num_bins = bins, tag::density::cache_size = 10)) {
}
