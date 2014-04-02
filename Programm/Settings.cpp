// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Settings.hpp"

#include <sstream>

std::string Settings::generate_filename(std::string prefix, std::string suffix) {
    std::ostringstream oss;

    oss << prefix;
    
    oss << iterations;
    oss << separator;

    oss << (fix_zeroth_coordinate ? "fix" : "nofix");

    oss << suffix;

    return oss.str();
}
