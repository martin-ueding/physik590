// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "ResultSet.hpp"
#include "Settings.hpp"

#include <iostream>
#include <random>
#include <thread>

typedef std::pair<const unsigned int, std::shared_ptr<Correlation>> correlation_pair;

ResultSet::ResultSet(BootstrapPool &pool) :
    dens {PositionDensity{ -5, 5, pool.histograms[0].size()}}, pool{pool} {
}

//unsigned int cpu_count = std::thread::hardware_concurrency();
