// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "ConcurrentCounter.hpp"

ConcurrentCounter::ConcurrentCounter(int max) : max {max} {
}

int ConcurrentCounter::operator()() {
    std::unique_lock<std::mutex> lock {mutex};
    int rval = max;
    max++;
    return rval;
}
