// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include <mutex>

class ConcurrentCounter {
    public:
        ConcurrentCounter(int max);

        int operator()();
    protected:
        std::mutex mutex;
        int max;
        int value {0};
};
