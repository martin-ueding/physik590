// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "parse_arguments.hpp"

#include <boost/program_options.hpp>

#include <iostream>

void parse_arguments(int argc, char **argv, Settings &settings) {
    boost::program_options::options_description options {"Program"};
    options.add_options()
    ("help,h", "Print usage and exit")
    ;

    boost::program_options::options_description group1 {
        "Simulation"
    };
    group1.add_options()
    ("time-sites",
     boost::program_options::value<unsigned>(&settings.time_sites)->default_value(settings.time_sites),
     "Number of sites in the time lattice")
    ("mass",
     boost::program_options::value<double>(&settings.mass)->default_value(settings.mass),
     "Mass")
    ("time-step",
     boost::program_options::value<double>(&settings.time_step)->default_value(settings.time_step),
     "Spacing of time lattice")
    ("mu",
     boost::program_options::value<double>(&settings.mu_squared)->default_value(settings.mu_squared),
     "μ²")
    ("inv-scatt-len",
     boost::program_options::value<double>(&settings.inverse_scattering_length)->default_value(settings.inverse_scattering_length),
     "Gauss height")
    ("gauss-width",
     boost::program_options::value<double>(&settings.gauss_width)->default_value(settings.gauss_width),
     "Gauss width")
    ("irw",
     boost::program_options::value<double>(&settings.initial_random_width)->default_value(settings.initial_random_width),
     "Initial random width")
    ("margin",
     boost::program_options::value<double>(&settings.margin)->default_value(settings.margin),
     "Random margin, Δ")
    ("iter",
     boost::program_options::value<unsigned>(&settings.iterations)->default_value(settings.iterations),
     "Iterations for the histogram")
    ("iter-pre",
     boost::program_options::value<unsigned>(&settings.pre_iterations)->default_value(settings.pre_iterations),
     "Iterations to relax the system")
    ("iter-between",
     boost::program_options::value<unsigned>(&settings.iterations_between)->default_value(settings.iterations_between),
     "Extra iterations between measurements")
    ("rounds",
     boost::program_options::value<unsigned>(&settings.rounds)->default_value(settings.rounds),
     "Rounds for a single x_j")
    ("rounds-pre",
     boost::program_options::value<unsigned>(&settings.pre_rounds)->default_value(settings.pre_rounds),
     "Rounds for a single x_j during relaxation")
    ("seed-pos",
     boost::program_options::value<int>(&settings.position_seed)->default_value(settings.position_seed),
     "Seed for the random number generator used for positions")
    ("seed-accept",
     boost::program_options::value<int>(&settings.accept_seed)->default_value(settings.accept_seed),
     "Seed for the random number generator used for accepting new positions")
    ("hist-bins-pos",
     boost::program_options::value<unsigned>(&settings.position_hist_bins)->default_value(settings.position_hist_bins),
     "Number of bins in the position histogram")
    ("hist-bins-action",
     boost::program_options::value<unsigned>(&settings.action_hist_bins)->default_value(settings.action_hist_bins),
     "Number of bins in the action histogram")
    ("corr-size",
     boost::program_options::value<unsigned>(&settings.correlation_size)->default_value(settings.correlation_size),
     "Size of the correlation matrix. Maximum eigenvalue is twice this.")
    ("corr-tau-max",
     boost::program_options::value<double>(&settings.corr_tau_max)->default_value(settings.corr_tau_max),
     "Last τ to calculate C(τ) for.")
    ("exp-pot-steps",
     boost::program_options::value<unsigned>(&settings.export_potential_steps)->default_value(settings.export_potential_steps),
     "Steps for potential export.")
    ("exp-pot-bound",
     boost::program_options::value<double>(&settings.export_potential_bound)->default_value(settings.export_potential_bound),
     "bounds for potential export.")
    ;
    options.add(group1);

    boost::program_options::options_description group3 {
        "Analysis"
    };
    group3.add_options()
    ("load",
     boost::program_options::value<std::string>(&settings.load_filename)->default_value(""),
     "Load this file")
    ("samples",
     boost::program_options::value<unsigned>(&settings.bootstrap_samples)->default_value(settings.bootstrap_samples),
     "Number of bootstrap samples to generate")
    ("t0",
     boost::program_options::value<unsigned>(&settings.t_0)->default_value(settings.t_0),
     "Start time for GEVP.")
    ;
    options.add(group3);

    boost::program_options::options_description group2 {
        "Common"
    };
    group2.add_options()
    ("max-cores",
     boost::program_options::value<unsigned>(&settings.max_cores)->default_value(settings.max_cores),
     "Maximum number of parallel threads.")
    ;
    options.add(group2);

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options), vm);
    boost::program_options::notify(vm);

    if (vm.count("help") > 0) {
        std::cout << options << std::endl;
        exit(0);
    }
}
