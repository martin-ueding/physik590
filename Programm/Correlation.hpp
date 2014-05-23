// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "Settings.hpp"

#include <eigen3/Eigen/Dense>

#include <vector>

/**
  Calculates the correlation matrix for a single trajectory.

  @param[in] x Trajectory
  @param[in] settings Global settings
  @param[in] distance Distance to calculate
  @param[in] even Calculate the even elements
  */
Eigen::MatrixXd correlation(std::vector<double> &x, Settings &settings,
                            const unsigned distance, const bool even);
