// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "MetropolisDriver.hpp"

#include <iostream>

MetropolisDriver::MetropolisDriver(Settings settings) :
    settings(settings),
    system(HarmonicOscillator {settings.time_step, settings.mass, settings.mu_squared}),
       trajectory(ListQuantity {settings.time_sites}),
ma(MetropolisAlgorithm {trajectory, system, settings.position_seed, settings.accept_seed}) {
}

void MetropolisDriver::run() {
    do_init();
    do_pre_iterations();
    do_iterations();
}

void MetropolisDriver::do_init() {
    trajectory.save_plot_file(settings.generate_filename("out/trajectory-01-init-", ".csv"));

    trajectory.set_to_random(settings.initial_random_width);
    trajectory.save_plot_file(settings.generate_filename("out/trajectory-02-random-", ".csv"));
}

void MetropolisDriver::do_pre_iterations() {
    for (int i = 0; i < settings.pre_iterations - settings.iterations_between; i++) {
        ma.iteration(settings.pre_rounds, settings.margin);
    }
    trajectory.save_plot_file(settings.generate_filename("out/trajectory-04-more_iterations-", ".csv"));
}

void MetropolisDriver::do_iterations() {
    VectorHistogram position_histogram {settings.position_hist_bins, settings.time_sites * settings.iterations};
    VectorHistogram action_histogram {settings.action_hist_bins, settings.iterations};

    ListQuantity action_list {settings.iterations};

    ma.reset_accept_rate();

    for (size_t i = 0; i < settings.iterations; i++) {
        for (int j = 0; j < settings.iterations_between; j++) {
            ma.iteration(settings.rounds, settings.margin);
        }
        ma.iteration(settings.rounds, settings.margin);
        trajectory.binning_snapshot(position_histogram);
        action_list.list[i] = system.action(trajectory.list);

        for (auto &x : trajectory.list) {
            moments.push(x);
        }

        if (i % 999 == 0) {
            std::cout << i * 100 / settings.iterations << "%" << std::endl;
        }
    }

    trajectory.save_plot_file(settings.generate_filename("out/trajectory-05-end-", ".csv"));
    position_histogram.save(settings.generate_filename("out/histogram-position-", ".csv"));

    action_list.binning_snapshot(action_histogram);
    action_list.save_plot_file(settings.generate_filename("out/trajectory-action-", ".csv"));
    action_histogram.save(settings.generate_filename("out/histogram-action-", ".csv"));

    std::cout << "Accept Rate Total:       " << ma.get_accept_rate() << std::endl;
    std::cout << "Accept Rate Negative:    " << ma.get_accept_rate_negative() << std::endl;
    std::cout << "Accept Rate Exponential: " << ma.get_accept_rate_exponential() << std::endl;

    std::cout << std::endl;

    for (size_t i{0}; i < moments.size(); i++) {
        std::cout << i << "\t" << moments[i] << std::endl;
    }


    std::cout << "σ\t" << moments.sigma() << std::endl;
    std::cout << "σ Theo\t" << moments.sigma_theory << std::endl;

    std::cout << "E₀\t" << system.ground_energy(moments) << std::endl;
}
