// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "BootstrappedHistogram.hpp"
#include "BootstrappedQuantity.hpp"
#include "BootstrapPool.hpp"
#include "Settings.hpp"

#include <atomic>

class Analysis {
    public:
        Analysis(BootstrapPool &pool, Settings &settings);

    protected:
        void insert_eigenvalues(CorrFunc &C, bool even);
        void create_samples();
        void save_eigenvalues();
        void save_c11();
        void save_histogram();
        void worker(ProgressBar &bar);

        BootstrapPool &pool;
        Settings &settings;

        BootstrappedHistogram boot_hist;

        std::vector<BootstrappedQuantity> c11;
        std::vector<BootstrappedQuantity> energies;

        std::atomic<unsigned> sample_id_atom {0};
};
