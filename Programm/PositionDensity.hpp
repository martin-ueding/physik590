// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "HistogramComputable.hpp"

class PositionDensity : public HistogramComputable {
        virtual void add_sample(BootstrapSample &sample);
        std::string get_name();
};
