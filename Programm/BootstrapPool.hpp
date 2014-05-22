// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "Histogram.hpp"
#include "MetropolisDriver.hpp"
#include "ProgressBar.hpp"
#include "Settings.hpp"

#include <eigen3/Eigen/Dense>

#include <atomic>
#include <map>
#include <mutex>
#include <vector>

typedef std::map<unsigned, Eigen::MatrixXd> CorrFunc;

/**
  The base set of trajectories that are used for bootstrapping later on.
  */
class BootstrapPool {
    public:
        /**
          Fills the pool with trajectories.

          @param[in] driver Driver that generates trajectories
          @param[in] settings Program settings
          */
        BootstrapPool(MetropolisDriver &driver, Settings &settings);

        void operator()(Settings &settings, ProgressBar &bar_corr);

        /**
          Gives the size of the pool.
          */
        size_t size() {
            return trajectories.size();
        }

        /**
          The original trajectories.
          */
        std::vector<std::vector<double>> trajectories;

        /**
          Pool of even correlation matrix function summands.

          Those are maps from the distances to the correlation matrix with that
          given distance. The matrix only contrains correlations for \c i and
          \c j that are even.
          */
        std::vector<CorrFunc> even;

        /**
          Same as \ref even, just for odd \c i and \c j.
          */
        std::vector<CorrFunc> odd;

        std::vector<Histogram> histograms;

        std::mt19937 engine;

    protected:
        std::atomic<unsigned> t_id_atom {0};
        std::mutex mutex;
};
