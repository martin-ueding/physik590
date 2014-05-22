// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "Histogram.hpp"
#include "MetropolisDriver.hpp"
#include "ProgressBar.hpp"
#include "Settings.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

#include <atomic>
#include <fstream>
#include <map>
#include <mutex>
#include <vector>

namespace boost {
    template<class Archive, typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
    inline void serialize(Archive &ar, Matrix < _Scalar, _Rows, _Cols, _Options,
                          _MaxRows, _MaxCols > &t,
                          const unsigned int file_version) {
        size_t rows = t.rows(), cols = t.cols();
        ar &rows;
        ar &cols;
        if (rows * cols != t.size()) {
            t.resize(rows, cols);
        }

        for (size_t i = 0; i < t.size(); i++) {
            ar &t.data()[i];
        }
    }
}

typedef std::map<unsigned, Eigen::MatrixXd> CorrFunc;

/**
  The base set of trajectories that are used for bootstrapping later on.
  */
class BootstrapPool {
        friend class boost::serialization::access;
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

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar &trajectories;
            ar &even;
            ar &odd;
            ar &histogram;
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
