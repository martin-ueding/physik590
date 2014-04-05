// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Settings.hpp"

#include <sstream>

std::string Settings::generate_filename(std::string prefix, std::string suffix) {
    std::ostringstream oss;

    oss << prefix;

    oss << "i{" << iterations << "}";
    oss << separator;
    oss << "o{" << mu_squared << "}";
    oss << separator;
    oss << "a{" << time_step << "}";
    oss << separator;
    oss << "s{" << position_seed << "}";
    oss << separator;
    oss << "c{" << accept_seed << "}";

    oss << suffix;

    return oss.str();
}
