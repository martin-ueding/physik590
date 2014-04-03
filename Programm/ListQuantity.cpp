// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "ListQuantity.hpp"

#include <fstream>
#include <functional>
#include <iostream>

ListQuantity::ListQuantity(size_t sites) : list(std::vector<double>(sites)) {
}

void ListQuantity::print() {
    for (double & x_j : list) {
        std::cout << x_j << std::endl;
    }
}

void ListQuantity::binning_snapshot(Histogram &histogram) {
    for (unsigned int i = 0; i < list.size(); i++) {
        histogram.push(list[i]);
    }
}

void ListQuantity::save_plot_file(std::string filename) {
    std::ofstream outfile(filename);
    outfile << "# j \t x_j" << std::endl;
    for (unsigned int i = 0; i < list.size(); i++) {
        outfile << i << "\t" << list[i] << std::endl;
    }
    outfile.close();
}

void ListQuantity::set_to_random(double bound) {
    std::uniform_real_distribution<double> distribution(-bound, bound);
    auto generator = std::bind(distribution, mt_engine);

    for (unsigned int i = 1; i < list.size() - 1; i++) {
        list[i] = generator();
    }
}
