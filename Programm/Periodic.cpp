// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

inline unsigned int Periodic::wrap(unsigned int number, unsigned int max) {
    return number % max;
}
