// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include <string>
#include <chrono>

class ProgressBar {
    public:
        ProgressBar(std::string prefix, unsigned max);
        ~ProgressBar();

        void update(unsigned value) {
            if (value % step_size == 0) {
                print_update(value);
            }
        }

        void close();

    protected:
        unsigned max;
        unsigned step_size;
        unsigned printed_steps {0};
        unsigned max_steps {70};

        bool is_closed {false};

        void print_update(unsigned value);

        std::chrono::time_point<std::chrono::steady_clock> start_time {std::chrono::steady_clock::now()};
};
