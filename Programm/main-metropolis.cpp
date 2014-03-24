// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "System.hpp"
#include "Trajectory.hpp"

#include <iostream>

int main() {
    System s;
    Trajectory t(10, s);

    t.print();
    t.set_to_random(40.);
    t.print();

    return 0;
}
