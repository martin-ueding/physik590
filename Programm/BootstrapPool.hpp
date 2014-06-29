// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

// http://stackoverflow.com/a/12618789
// http://stackoverflow.com/a/18423496

#pragma once

#include "Histogram.hpp"
#include "MetropolisDriver.hpp"
#include "ProgressBar.hpp"
#include "Settings.hpp"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

#include <atomic>
#include <mutex>

namespace boost {
    template<class Archive, typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
    inline void serialize(Archive &ar, Eigen::Matrix < _Scalar, _Rows, _Cols, _Options,
                          _MaxRows, _MaxCols > &t,
                          const unsigned int file_version) {
        long rows = t.rows(), cols = t.cols();
        ar &rows;
        ar &cols;
        if (rows * cols != t.size()) {
            t.resize(rows, cols);
        }

        for (long i = 0; i < t.size(); i++) {
            ar &t.data()[i];
        }
    }
#pragma clang diagnostic pop
}

typedef std::vector<Eigen::MatrixXd> CorrList;

/**
  The base set of trajectories that are used for bootstrapping later on.
  */
class BootstrapPool {
        friend class boost::serialization::access;
    public:
        BootstrapPool();

        /**
          Fills the pool with trajectories.

          @param[in] driver Driver that generates trajectories
          @param[in] settings Program settings
          */
        BootstrapPool(MetropolisDriver &driver, Settings &settings);

        void worker(Settings settings, ProgressBar &bar_corr, MetropolisDriver driver, int seed);

        /**
          Gives the size of the pool.
          */
        size_t size() {
            return histograms.size();
        }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
        template<class Archive>
        void serialize(Archive &ar, const unsigned version) {
            ar &even;
            ar &odd;
            ar &histograms;
        }
#pragma clang diagnostic pop

        /**
          Pool of even correlation matrix function summands.

          Those are maps from the distances to the correlation matrix with that
          given distance. The matrix only contrains correlations for \c i and
          \c j that are even.
          */
        std::vector<CorrList> even;

        /**
          Same as \ref even, just for odd \c i and \c j.
          */
        std::vector<CorrList> odd;

        std::vector<Histogram> histograms;

        std::vector<double> e0_virial;

        std::mt19937 engine;

    protected:
        std::atomic<unsigned> t_id_atom {0};
        std::mutex mutex;

        std::ostringstream accept_rate_output;
        std::mutex accept_rate_mutex;
};

void save_pool(std::shared_ptr<BootstrapPool> pool, Settings &settings);

void load_into_pool(std::shared_ptr<BootstrapPool> &pool, Settings &settings);
