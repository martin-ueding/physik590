// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include <string>

class ProgressBar {
    public:
        ProgressBar(std::string prefix, size_t max);
        ~ProgressBar();

        void update(int value) {
            if (value % step_size == 0) {
                print_update(value);
            }
        }

        void close();

    protected:
        size_t max;
        size_t step_size;
        int printed_steps {0};
        int max_steps {70};

        bool is_closed {false};

        void print_update(int value);
};
