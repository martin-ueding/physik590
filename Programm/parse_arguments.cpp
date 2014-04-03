// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "parse_arguments.hpp"

#include <boost/program_options.hpp>

#include <iostream>

bool parse_arguments(int argc, char **argv, Settings &settings) {
    boost::program_options::options_description options("Program options");
    options.add_options()
    ("help,h", "Print usage and exit")
    ;

    boost::program_options::options_description oszillator_options("Oszillator options");
    oszillator_options.add_options()
    ("time-bins,t", boost::program_options::value<size_t>(&settings.time_sites)->default_value(1000), "Number of sites in the time lattice")
    ("mass,m", boost::program_options::value<double>(&settings.mass)->default_value(1), "Mass")
    ("time-step,t", boost::program_options::value<double>(&settings.time_step)->default_value(0.1), "Spacing of time lattice")
    ("mu-squared,o", boost::program_options::value<double>(&settings.mu_squared)->default_value(1), "μ²")
    ;
    options.add(oszillator_options);

    boost::program_options::options_description init_options("Initialization options");
    init_options.add_options()
    ("irw", boost::program_options::value<double>(&settings.initial_random_width)->default_value(0.63), "Initial random width")
    ("margin", boost::program_options::value<double>(&settings.margin)->default_value(0.632456), "Random margin, Δ")
    ("pre-iterations", boost::program_options::value<int>(&settings.pre_iterations)->default_value(50), "Iterations to relax the system")
    ("pre-rounds", boost::program_options::value<int>(&settings.pre_rounds)->default_value(5), "Rounds for a single x_j during relaxation")
    ;
    options.add(init_options);

    boost::program_options::options_description iter_options("Iteration options");
    iter_options.add_options()
    ("iterations,i", boost::program_options::value<size_t>(&settings.iterations)->default_value(10000), "Iterations for the histogram")
    ("rounds,r", boost::program_options::value<int>(&settings.rounds)->default_value(5), "Rounds for a single x_j")
    ("iterations-between", boost::program_options::value<int>(&settings.iterations_between)->default_value(2), "Extra iterations between measurements")
    ("fix-zero", "Fix x₀")
    ;
    options.add(iter_options);

    boost::program_options::options_description hist_options("Histogram options");
    hist_options.add_options()
    ("position-hist-bins,b", boost::program_options::value<size_t>(&settings.position_hist_bins)->default_value(1000), "Number of bins in the position histogram")
    ("action-hist-bins", boost::program_options::value<size_t>(&settings.action_hist_bins)->default_value(100), "Number of bins in the action histogram")
    ;
    options.add(hist_options);

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options), vm);
    boost::program_options::notify(vm);

    if (vm.count("help") > 0) {
        std::cout << options << std::endl;
        return true;
    }

    if (vm.count("fix-zero") > 0) {
        settings.fix_zeroth_coordinate = true;
    }

    return false;
}
