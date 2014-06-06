// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "BootstrappedHistogram.hpp"
#include "BootstrappedQuantity.hpp"
#include "BootstrapPool.hpp"
#include "Settings.hpp"

#include <atomic>

typedef std::map<unsigned, std::map<unsigned, BootstrappedQuantity>> BQMapMap;

class Analysis {
    public:
        Analysis(BootstrapPool &pool, Settings &settings);

    protected:
        void insert_eigenvalues(CorrFunc &C, bool even, BQMapMap &bs_lambda_n_t);
        void create_samples();
        void save_eigenvalues();
        void save_c11();
        void save_c22();
        void save_histogram();
        void worker(ProgressBar &bar);

        BootstrapPool &pool;
        Settings &settings;

        BootstrappedHistogram boot_hist;

        /**
          The inner map is a mapping that goes through all E_n. The outter map
          holds the E_n at different times t.
          */
        BQMapMap bs_E_n_t;

        std::vector<BootstrappedQuantity> c11;
        std::vector<BootstrappedQuantity> c22;

        std::atomic<unsigned> sample_id_atom {0};
};
