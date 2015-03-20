// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Periodic.hpp"

#include <iostream>

unsigned int Periodic::wrap(int number, unsigned int max) {
    unsigned int result;

    if (number < 0) {
        result = number + max;
    } else if ((unsigned int)number >= max) {
        result = number % max;
    } else {
        result = number;
    }

    return result;
}
