// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

#ifndef PERIODIC_H
#define PERIODIC_H

/**
  Helpers for periodic boundary conditions.
  */
class Periodic {
    public:
        /**
          Wraps the number into the maximum size.

          This method will make it possible to access the @f$ N+1 @f$-th
          element of a container with size @f$ N @f$. Also @f$ -1 @f$ and other
          indices are supported.

          @param number Index to fit
          @param max Container size
          */
        static unsigned int wrap(int number, unsigned int max);
};

#endif /* end of include guard: PERIODIC_H */
