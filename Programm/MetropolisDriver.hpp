// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef METROPOLISDRIVER_H
#define METROPOLISDRIVER_H

#include "Settings.hpp"

class MetropolisDriver {
    public:
        MetropolisDriver(Settings &&settings);

    protected:
        Settings &settings;
};

#endif /* end of include guard: METROPOLISDRIVER_H */
