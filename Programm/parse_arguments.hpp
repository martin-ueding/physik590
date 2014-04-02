// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef PARSE_ARGUMENTS_H
#define PARSE_ARGUMENTS_H

#include "Settings.hpp"

/**
  Parses the command line arguments.

  The return value determines whether the program needs to be halted after this
  function. That will be needed after printing the options.

  @param[in] argc Argument count
  @param[int] argv Argument values
  @param[out] settings Settings instance
  @return Whether program should be halted
  */
bool parse_arguments(int argc, char **argv, Settings &settings);

#endif /* end of include guard: PARSE_ARGUMENTS_H */
