// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

#include "HarmonicOszillator.hpp"
#include "Histogram.hpp"
#include "Trajectory.hpp"

#include <boost/program_options.hpp>

#include <iostream>

int main(int argc, char **argv) {
    int bins;

    boost::program_options::options_description description("");
    description.add_options()
    ("help,h", "Print usage and exit")
    ("bins",  boost::program_options::value<int>(&bins)->default_value(100), "Number of bins in the histogram")
    ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, description), vm);
    boost::program_options::notify(vm);

    if (vm.count("help") > 0) {
        std::cout << description << std::endl;
        return 0;
    }

    HarmonicOszillator ho;
    Trajectory t(bins, ho);

    t.save_plot_file("trajectory-01-init.csv");
    std::cout << "Action after init: " << t.action() << std::endl;

    t.set_to_random(40.);
    t.save_plot_file("trajectory-02-random.csv");
    std::cout << "Action after random: " << t.action() << std::endl;

    t.iteration(1, 40.);
    t.save_plot_file("trajectory-03-iteration.csv");
    std::cout << "Action after iteration: " << t.action() << std::endl;

    for (int i = 0; i < 1000; i++) {
        t.iteration(5, 40.);
    }
    t.save_plot_file("trajectory-04-more_iterations.csv");
    std::cout << "Action after more iterations: " << t.action() << std::endl;

    Histogram histogram(100, 49*1000);

    for (int i = 0; i < 10000; i++) {
        t.iteration(5, 40.);
        t.binning_snapshot(histogram);
    }

    histogram.save("histogram-1000.csv");

    return 0;
}
