// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "BootstrappedQuantity.hpp"
#include "Computable.hpp"

class ScalarComputable : public Computable, public BootstrappedQuantity {
};
