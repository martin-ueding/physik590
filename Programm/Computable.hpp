// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "BootstrapSample.hpp"

#include <boost/serialization/string.hpp>

class Computable {
    public:
        virtual void add_sample(BootstrapSample &sample) = 0;
        virtual std::string get_name();
};
