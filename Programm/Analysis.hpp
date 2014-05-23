// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "BootstrappedQuantity.hpp"
#include "BootstrapPool.hpp"
#include "Settings.hpp"

typedef std::map<unsigned, std::map<unsigned, BootstrappedQuantity>> BQMapMap;

class Analysis {
    public:
        Analysis(BootstrapPool &pool, Settings &settings);

    protected:
        void insert_eigenvalues(CorrFunc &C, bool even, BQMapMap &bs_lambda_n_t);

        BootstrapPool &pool;
        Settings &settings;
};
