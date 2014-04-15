// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "MetropolisDriver.hpp"

#include <iostream>

MetropolisDriver::MetropolisDriver(Settings settings)
    :
        settings(settings),
        system(HarmonicOscillator {settings.time_step, settings.mass, settings.mu_squared}),
        trajectory(ListQuantity {settings.time_sites}),
        ma(MetropolisAlgorithm {trajectory, system, settings.position_seed, settings.accept_seed}) {

    trajectory.save_plot_file(settings.generate_filename("out/trajectory-01-init-", ".csv"));

    trajectory.set_to_random(settings.initial_random_width);
    trajectory.save_plot_file(settings.generate_filename("out/trajectory-02-random-", ".csv"));

    for (int i = 0; i < settings.pre_iterations - settings.iterations_between; i++) {
        ma.iteration(settings.pre_rounds, settings.margin);
    }
    trajectory.save_plot_file(settings.generate_filename("out/trajectory-04-more_iterations-", ".csv"));
}

Trajectory MetropolisDriver::next() {
    ma.reset_accept_rate();

    for (int j = 0; j < settings.iterations_between; j++) {
        ma.iteration(settings.rounds, settings.margin);
    }
    ma.iteration(settings.rounds, settings.margin);

    return Trajectory{ma.x};
}
