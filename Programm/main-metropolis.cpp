// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

#include "BoostHistogram.hpp"
#include "HarmonicOszillator.hpp"
#include "MetropolisAlgorithm.hpp"
#include "parse_arguments.hpp"
#include "Settings.hpp"

#include <iostream>

/**
  Initializes the trajectory to random.

  @param[in] settings Command line options
  @param[in,out] trajectory List with @f$ x @f$ values
  */
void do_init(Settings &settings, ListQuantity &trajectory) {
    trajectory.save_plot_file(settings.generate_filename("out/trajectory-01-init-", ".csv"));

    trajectory.set_to_random(settings.initial_random_width);
    trajectory.save_plot_file(settings.generate_filename("out/trajectory-02-random-", ".csv"));
}

void do_pre_iterations(Settings &settings, ListQuantity &trajectory,
                       MetropolisAlgorithm &ma) {
    for (int i = 0; i < settings.pre_iterations - settings.iterations_between; i++) {
        ma.iteration(settings.pre_rounds, settings.margin, settings.fix_zeroth_coordinate);
    }
    trajectory.save_plot_file(settings.generate_filename("out/trajectory-04-more_iterations-", ".csv"));
}

void do_iterations(Settings &settings, ListQuantity &trajectory,
                   MetropolisAlgorithm &ma, System &system) {
    BoostHistogram position_histogram{settings.position_hist_bins, settings.time_sites * settings.iterations};
    BoostHistogram action_histogram{settings.action_hist_bins, settings.iterations};

    ListQuantity action_list(settings.iterations);

    for (int i = 0; i < 50; i++) {
        std::cout << "-";
    }
    std::cout << std::endl;

    for (size_t i = 0; i < settings.iterations; i++) {
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

    trajectory.save_plot_file(settings.generate_filename("out/trajectory-05-end-", ".csv"));
    position_histogram.save(settings.generate_filename("out/histogram-position-", ".csv"));

    //action_list.binning_snapshot(action_histogram);
    action_list.save_plot_file(settings.generate_filename("out/trajectory-action-", ".csv"));
    //action_histogram.save("out/histogram-action-1000.csv");
}

/**
  Entry point for the metropolis program.

  @param argc Argument count
  @param argv Argument values
  @return Return code
  */
int main(int argc, char **argv) {
    Settings settings;

    if (parse_arguments(argc, argv, settings)) {
        return 0;
    }

    HarmonicOszillator ho(settings.time_step, settings.mass, settings.mu_squared);
    ListQuantity trajectory(settings.time_sites);
    MetropolisAlgorithm ma(trajectory, ho);

    do_init(settings, trajectory);
    do_pre_iterations(settings, trajectory, ma);
    do_iterations(settings, trajectory, ma, ho);

    return 0;
}
