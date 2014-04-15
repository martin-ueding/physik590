// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "Moment.hpp"
#include "ScalarComputable.hpp"

class ResultSet {
    public:
        Moment mean {1};
        std::vector<ScalarComputable*> computables;
};
