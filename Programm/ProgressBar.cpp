// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "ProgressBar.hpp"

#include <iostream>

ProgressBar::ProgressBar(std::string prefix, size_t max) : max {max} {
    step_size = max / max_steps;
    std::cout << prefix << " …" << std::endl;

    std::cout << "[";
    for (int i = 0; i < max_steps; i++) {
        std::cout << "-";
    }
    std::cout << "]" << std::flush;
    for (int i = 0; i < max_steps + 1; i++) {
        std::cout << "\b";
    }
    std::cout << std::flush;
}

ProgressBar::~ProgressBar() {
    close();
}

void ProgressBar::print_update(int value) {
    int target_steps = value * max_steps / max;

    for (; target_steps > printed_steps; printed_steps++) {
        std::cout << "=";
    }
    std::cout << std::flush;
}

void ProgressBar::close() {
    if (is_closed) {
        return;
    }
    is_closed = true;

    print_update(max);

    std::cout << "]";

    double time_in_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::steady_clock::now() - start_time).count() / 1000.0;
    std::cout << " " << time_in_seconds << " s";

    std::cout << std::endl;
}
