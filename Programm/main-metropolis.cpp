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
    int time_bins;
    int hist_bins;
    double initial_random_width;
    double margin;
    int iterations;
    int pre_iterations;
    int pre_rounds;
    int rounds;


    boost::program_options::options_description description("");
    description.add_options()
    ("help,h", "Print usage and exit")
    ("time-bins,t",  boost::program_options::value<int>(&time_bins)->default_value(100), "Number of bins in the time lattice")
    ("hist-bins,b",  boost::program_options::value<int>(&hist_bins)->default_value(100), "Number of bins in the histogram")
    ("irw",  boost::program_options::value<double>(&initial_random_width)->default_value(40), "Initial random width")
    ("margin,m",  boost::program_options::value<double>(&margin)->default_value(40), "Random margin, Δ")
    ("iterations,i",  boost::program_options::value<int>(&iterations)->default_value(10000), "Iterations for the histogram")
    ("rounds,r",  boost::program_options::value<int>(&rounds)->default_value(5), "Rounds for a single x_j")
    ("pre-iterations",  boost::program_options::value<int>(&pre_iterations)->default_value(1000), "Iterations to relax the system")
    ("pre-rounds",  boost::program_options::value<int>(&pre_rounds)->default_value(5), "Rounds for a single x_j during relaxation")
    ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, description), vm);
    boost::program_options::notify(vm);

    if (vm.count("help") > 0) {
        std::cout << description << std::endl;
        return 0;
    }

    HarmonicOszillator ho;
    Trajectory t(time_bins, ho);

    t.save_plot_file("trajectory-01-init.csv");
    std::cout << "Action after init: " << t.action() << std::endl;

    t.set_to_random(initial_random_width);
    t.save_plot_file("trajectory-02-random.csv");
    std::cout << "Action after random: " << t.action() << std::endl;

    t.iteration(pre_rounds, margin);
    t.save_plot_file("trajectory-03-iteration.csv");
    std::cout << "Action after iteration: " << t.action() << std::endl;

    for (int i = 0; i < pre_iterations; i++) {
        t.iteration(pre_rounds, margin);
    }
    t.save_plot_file("trajectory-04-more_iterations.csv");
    std::cout << "Action after more iterations: " << t.action() << std::endl;

    Histogram histogram(hist_bins, (time_bins-1)*iterations);

    for (int i = 0; i < iterations; i++) {
        t.iteration(rounds, margin);
        t.binning_snapshot(histogram);
    }

    histogram.save("histogram-1000.csv");

    return 0;
}
