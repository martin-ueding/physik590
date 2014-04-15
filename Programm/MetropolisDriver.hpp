// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "BoostHistogram.hpp"
#include "HarmonicOscillator.hpp"
#include "ListQuantity.hpp"
#include "Trajectory.hpp"
#include "MetropolisAlgorithm.hpp"
#include "Settings.hpp"
#include "VectorHistogram.hpp"

/**
  Driver for the MetropolisAlgorithm.

  This creates a HarmonicOscillator, a MetropolisAlgorithm and a ListQuantity
  to keep track of the trajectory.

  The settings are copied, so that there is no shared data between instances of
  this class and the caller. This makes it possible to have multiple drivers
  run different simulations asynchronously.
  */
class MetropolisDriver {
    public:
        /**
          Creates a new driver with the given settings.

          @param settings[in] Collection of settings
          */
        MetropolisDriver(Settings settings);

        Trajectory next();

    protected:
        Settings settings;
        HarmonicOscillator system;
        ListQuantity trajectory;
        MetropolisAlgorithm ma;
};
