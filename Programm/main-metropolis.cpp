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

    double mass;
    double time_step;
    double omega_squared;

    boost::program_options::options_description options("Program options");
    options.add_options()
    ("help,h", "Print usage and exit")
    ("time-bins,t",  boost::program_options::value<int>(&time_bins)->default_value(100), "Number of bins in the time lattice")
    ;

    boost::program_options::options_description oszillator_options("Oszillator options");
    oszillator_options.add_options()
    ("mass,m",  boost::program_options::value<double>(&mass)->default_value(1), "Mass")
    ("time-step,t",  boost::program_options::value<double>(&time_step)->default_value(1), "Time step")
    ("omega-squared,o",  boost::program_options::value<double>(&omega_squared)->default_value(1), "ω²")
    ;
    options.add(oszillator_options);

    boost::program_options::options_description init_options("Initialization options");
    init_options.add_options()
    ("irw",  boost::program_options::value<double>(&initial_random_width)->default_value(40), "Initial random width")
    ("margin",  boost::program_options::value<double>(&margin)->default_value(40), "Random margin, Δ")
    ("pre-iterations",  boost::program_options::value<int>(&pre_iterations)->default_value(1000), "Iterations to relax the system")
    ("pre-rounds",  boost::program_options::value<int>(&pre_rounds)->default_value(5), "Rounds for a single x_j during relaxation")
    ;
    options.add(init_options);

    boost::program_options::options_description iter_options("Iteration options");
    iter_options.add_options()
    ("iterations,i",  boost::program_options::value<int>(&iterations)->default_value(10000), "Iterations for the histogram")
    ("rounds,r",  boost::program_options::value<int>(&rounds)->default_value(5), "Rounds for a single x_j")
    ;
    options.add(iter_options);

    boost::program_options::options_description hist_options("Histogram options");
    hist_options.add_options()
    ("hist-bins,b",  boost::program_options::value<int>(&hist_bins)->default_value(100), "Number of bins in the histogram")
    ;
    options.add(hist_options);

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options), vm);
    boost::program_options::notify(vm);

    if (vm.count("help") > 0) {
        std::cout << options << std::endl;
        return 0;
    }

    HarmonicOszillator ho(time_step, mass, omega_squared);
    Trajectory t(time_bins, ho);

    t.save_plot_file("trajectory-01-init.csv");

    t.set_to_random(initial_random_width);
    t.save_plot_file("trajectory-02-random.csv");

    t.iteration(pre_rounds, margin);
    t.save_plot_file("trajectory-03-iteration.csv");

    for (int i = 0; i < pre_iterations; i++) {
        t.iteration(pre_rounds, margin);
    }
    t.save_plot_file("trajectory-04-more_iterations.csv");

    Histogram histogram(hist_bins, (time_bins-1)*iterations);

    for (int i = 0; i < iterations; i++) {
        t.iteration(rounds, margin);
        t.binning_snapshot(histogram);
    }

    histogram.save("histogram-1000.csv");

    return 0;
}
