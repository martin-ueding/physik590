// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Histogram.hpp"

#include "SizePrinter.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

void Histogram::print() {
    write_histogram(std::cout);
}

void Histogram::save(std::string filename) {
    std::ofstream outfile(filename);
    write_histogram(outfile);
    outfile.close();
}
