// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

#include "HarmonicOszillator.hpp"
#include "Histogram.hpp"
#include "MetropolisAlgorithm.hpp"
#include "Settings.hpp"

#include <boost/program_options.hpp>

#include <iostream>

/**
  Initializes the trajectory to random.

  @param[in] settings Command line options
  @param[in,out] trajectory List with @f$ x @f$ values
  */
void do_init(Settings &settings, ListQuantity &trajectory) {
    trajectory.save_plot_file("out/trajectory-01-init.csv");

    trajectory.set_to_random(settings.initial_random_width);
    trajectory.save_plot_file("out/trajectory-02-random.csv");
}

void do_pre_iterations(Settings &settings, ListQuantity &trajectory,
                       MetropolisAlgorithm &ma) {
    for (int i = 0; i < settings.pre_iterations - settings.iterations_between; i++) {
        ma.iteration(settings.pre_rounds, settings.margin, settings.fix_zeroth_coordinate);
    }
    trajectory.save_plot_file("out/trajectory-04-more_iterations.csv");
}

void do_iterations(Settings &settings, ListQuantity &trajectory,
                   MetropolisAlgorithm &ma, System &system) {
    Histogram position_histogram(settings.position_hist_bins, settings.time_sites * settings.iterations);
    Histogram action_histogram(settings.action_hist_bins, settings.iterations);

    ListQuantity action_list(settings.iterations);

    for (int i = 0; i < 50; i++) {
        std::cout << "-";
    }
    std::cout << std::endl;

    for (int i = 0; i < settings.iterations; i++) {
        for (int j = 0; j < settings.iterations_between; j++) {
            ma.iteration(settings.rounds, settings.margin, settings.fix_zeroth_coordinate);
        }
        ma.iteration(settings.rounds, settings.margin, settings.fix_zeroth_coordinate);
        trajectory.binning_snapshot(position_histogram);
        action_list.list[i] = system.action(trajectory.list);

        if (i * 50 % settings.iterations == 0) {
            std::cout << "=" << std::flush;
        }
    }

    std::cout << std::endl;

    trajectory.save_plot_file("out/trajectory-05-end.csv");
    position_histogram.save("out/histogram-position-1000.csv");

    action_list.binning_snapshot(action_histogram);
    action_list.save_plot_file("out/trajectory-action.csv");
    action_histogram.save("out/histogram-action-1000.csv");
}

/**
  Entry point for the metropolis program.

  @param argc Argument count
  @param argv Arguments
  */
int main(int argc, char **argv) {
    Settings settings;

    boost::program_options::options_description options("Program options");
    options.add_options()
    ("help,h", "Print usage and exit")
    ;

    boost::program_options::options_description oszillator_options("Oszillator options");
    oszillator_options.add_options()
    ("time-bins,t",  boost::program_options::value<int>(&settings.time_sites)->default_value(1000), "Number of sites in the time lattice")
    ("mass,m",  boost::program_options::value<double>(&settings.mass)->default_value(1), "Mass")
    ("time-step,t",  boost::program_options::value<double>(&settings.time_step)->default_value(0.1), "Spacing of time lattice")
    ("mu-squared,o",  boost::program_options::value<double>(&settings.mu_squared)->default_value(1), "μ²")
    ;
    options.add(oszillator_options);

    boost::program_options::options_description init_options("Initialization options");
    init_options.add_options()
    ("irw",  boost::program_options::value<double>(&settings.initial_random_width)->default_value(0.63), "Initial random width")
    ("margin",  boost::program_options::value<double>(&settings.margin)->default_value(0.632456), "Random margin, Δ")
    ("pre-iterations",  boost::program_options::value<int>(&settings.pre_iterations)->default_value(50), "Iterations to relax the system")
    ("pre-rounds",  boost::program_options::value<int>(&settings.pre_rounds)->default_value(5), "Rounds for a single x_j during relaxation")
    ;
    options.add(init_options);

    boost::program_options::options_description iter_options("Iteration options");
    iter_options.add_options()
    ("iterations,i",  boost::program_options::value<int>(&settings.iterations)->default_value(10000), "Iterations for the histogram")
    ("rounds,r",  boost::program_options::value<int>(&settings.rounds)->default_value(5), "Rounds for a single x_j")
    ("iterations-between",  boost::program_options::value<int>(&settings.iterations_between)->default_value(2), "Extra iterations between measurements")
    ("fix-zeroth-coordinate", "Fix x₀")
    ;
    options.add(iter_options);

    boost::program_options::options_description hist_options("Histogram options");
    hist_options.add_options()
    ("position-hist-bins,b",  boost::program_options::value<int>(&settings.position_hist_bins)->default_value(1000), "Number of bins in the position histogram")
    ("action-hist-bins,b",  boost::program_options::value<int>(&settings.action_hist_bins)->default_value(100), "Number of bins in the action histogram")
    ;
    options.add(hist_options);

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options), vm);
    boost::program_options::notify(vm);

    if (vm.count("help") > 0) {
        std::cout << options << std::endl;
        return 0;
    }

    if (vm.count("fix-zeroth-coordinate") > 0) {
        settings.fix_zeroth_coordinate = true;
    }

    HarmonicOszillator ho(settings.time_step, settings.mass, settings.mu_squared);
    ListQuantity trajectory(settings.time_sites);
    MetropolisAlgorithm ma(trajectory, ho);

    do_init(settings, trajectory);
    do_pre_iterations(settings, trajectory, ma);
    do_iterations(settings, trajectory, ma, ho);

    return 0;
}

