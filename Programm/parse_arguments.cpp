// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "parse_arguments.hpp"

#include <boost/program_options.hpp>

#include <iostream>

bool parse_arguments(int argc, char **argv, Settings &settings) {
    boost::program_options::options_description options {"Program options"};
    options.add_options()
    ("help,h", "Print usage and exit")
    ;

    boost::program_options::options_description oszillator_options {
        "Oscillator options"
    };
    oszillator_options.add_options()
    ("time-bins,t",
     boost::program_options::value<unsigned>(&settings.time_sites)->default_value(settings.time_sites),
     "Number of sites in the time lattice")
    ("mass,m",
     boost::program_options::value<double>(&settings.mass)->default_value(settings.mass),
     "Mass")
    ("time-step,a",
     boost::program_options::value<double>(&settings.time_step)->default_value(settings.time_step),
     "Spacing of time lattice")
    ("mu-squared,o",
     boost::program_options::value<double>(&settings.mu_squared)->default_value(settings.mu_squared),
     "μ²")
    ("inverse-scattering-length",
     boost::program_options::value<double>(&settings.inverse_scattering_length)->default_value(settings.inverse_scattering_length),
     "Gauss height")
    ("gauss-width",
     boost::program_options::value<double>(&settings.gauss_width)->default_value(settings.gauss_width),
     "Gauss width")
    ;
    options.add(oszillator_options);

    boost::program_options::options_description init_options {
        "Initialization options"
    };
    init_options.add_options()
    ("irw",
     boost::program_options::value<double>(&settings.initial_random_width)->default_value(settings.initial_random_width),
     "Initial random width")
    ("margin",
     boost::program_options::value<double>(&settings.margin)->default_value(settings.margin),
     "Random margin, Δ")
    ("pre-iterations",
     boost::program_options::value<unsigned>(&settings.pre_iterations)->default_value(settings.pre_iterations),
     "Iterations to relax the system")
    ("pre-rounds",
     boost::program_options::value<unsigned>(&settings.pre_rounds)->default_value(settings.pre_rounds),
     "Rounds for a single x_j during relaxation")
    ;
    options.add(init_options);

    boost::program_options::options_description iter_options {
        "Iteration options"
    };
    iter_options.add_options()
    ("iterations,i",
     boost::program_options::value<unsigned>(&settings.iterations)->default_value(settings.iterations),
     "Iterations for the histogram")
    ("rounds,r",
     boost::program_options::value<unsigned>(&settings.rounds)->default_value(settings.rounds),
     "Rounds for a single x_j")
    ("iterations-between",
     boost::program_options::value<unsigned>(&settings.iterations_between)->default_value(settings.iterations_between),
     "Extra iterations between measurements")
    ("position-seed,s",
     boost::program_options::value<int>(&settings.position_seed)->default_value(settings.position_seed),
     "Seed for the random number generator used for positions")
    ("accept-seed,c",
     boost::program_options::value<int>(&settings.accept_seed)->default_value(settings.accept_seed),
     "Seed for the random number generator used for accepting new positions")
    ("samples",
     boost::program_options::value<unsigned>(&settings.bootstrap_samples)->default_value(settings.bootstrap_samples),
     "Number of bootstrap samples to generate")
    ;
    options.add(iter_options);

    boost::program_options::options_description hist_options {
        "Histogram options"
    };
    hist_options.add_options()
    ("position-hist-bins,b",
     boost::program_options::value<unsigned>(&settings.position_hist_bins)->default_value(settings.position_hist_bins),
     "Number of bins in the position histogram")
    ("action-hist-bins",
     boost::program_options::value<unsigned>(&settings.action_hist_bins)->default_value(settings.action_hist_bins),
     "Number of bins in the action histogram")
    ("correlation-size",
     boost::program_options::value<unsigned>(&settings.correlation_size)->default_value(settings.correlation_size),
     "Size of the correlation matrix. Maximum eigenvalue is twice this.")
    ;
    options.add(hist_options);

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options), vm);
    boost::program_options::notify(vm);

    if (vm.count("help") > 0) {
        std::cout << options << std::endl;
        return true;
    }

    return false;
}
