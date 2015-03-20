// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "VectorHelper.hpp"

#include <fstream>
#include <functional>

void VectorHelper::save_plot_file(std::vector<double> &list,
                                  std::string filename,
                                  std::string preamble) {
    std::ofstream outfile{filename};
    outfile << preamble;
    outfile << "# j \t x_j" << std::endl;
    for (unsigned int i = 0; i < list.size(); i++) {
        outfile << i << "\t" << list[i] << std::endl;
    }
    outfile.close();
}

void VectorHelper::set_to_random(std::vector<double> &list, double bound) {
    std::uniform_real_distribution<double> distribution(-bound, bound);
    auto generator = std::bind(distribution, mt_engine);

    for (unsigned int i = 1; i < list.size() - 1; i++) {
        list[i] = generator();
    }
}
