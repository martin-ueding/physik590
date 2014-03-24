// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Trajectory.hpp"
#include "System.hpp"

#include <iostream>

int main() {
    System s;
    Trajectory t(100, s);

    t.print();

    return 0;
}
