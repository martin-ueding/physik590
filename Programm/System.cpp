// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "System.hpp"

System::System(double time_step, double mass) : time_step(time_step), mass(mass) {
}

double System::action(std::vector<double> &x) {
    double sum = 0.0;

    for (unsigned int i = 0; i < x.size() - 1; i++) {
        sum += action_step(x[i], x[i + 1]);
    }

    return sum;
}

double System::action_difference(double prev, double cur, double alt,
                                 double next) {
    double action_cur = action_step(prev, cur) + action_step(cur, next);
    double action_alt = action_step(prev, alt) + action_step(cur, alt);

    return action_alt - action_cur;
}

double System::action_step(double cur, double next) {
    double difference = next - cur;
    double difference_squared = difference * difference;
    return 0.5 * mass * difference_squared / (time_step * time_step) + potential(cur);
}
