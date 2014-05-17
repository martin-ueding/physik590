// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "MetropolisDriver.hpp"

#include "VectorHelper.hpp"

MetropolisDriver::MetropolisDriver(Settings settings) :
    settings{settings},
    system{Oscillator {settings}},
x {std::vector<double>(settings.time_sites)},
ma{MetropolisAlgorithm {x, system, settings.position_seed, settings.accept_seed}} {

    system.export_potential(settings.generate_filename("potential.csv"), settings.report(), settings);

    VectorHelper vh;

    vh.save_plot_file(x, settings.generate_filename("trajectory-01-init.csv"), settings.report());

    vh.set_to_random(x, settings.initial_random_width);
    vh.save_plot_file(x, settings.generate_filename("trajectory-02-random.csv"), settings.report());

    for (unsigned i = 0; i < settings.pre_iterations - settings.iterations_between; i++) {
        ma.iteration(settings.pre_rounds, settings.margin);
    }
    vh.save_plot_file(x, settings.generate_filename("trajectory-04-more_iterations.csv"), settings.report());
}

std::vector<double> MetropolisDriver::next() {
    for (unsigned j = 0; j < settings.iterations_between; j++) {
        ma.iteration(settings.rounds, settings.margin);
    }
    ma.iteration(settings.rounds, settings.margin);

    return x;
}
