// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "SizePrinter.hpp"

#include <iostream>
#include <sstream>

std::string SizePrinter::format(size_t size) {
    int level = find_level(size);
    double mantissa = size;

    for (int i = 0; i < level; i++) {
        mantissa /= 1024.0;
    }

    std::ostringstream oss;

    oss << mantissa << " " << suffixes[level];
    return oss.str();
}

void SizePrinter::print(size_t size) {
    std::cout << format(size) << std::endl;
}

int SizePrinter::find_level(size_t size) {
    int level = 0;
    while (size > 1024) {
        size /= 1024;
        level++;
    }
    return level;
}
