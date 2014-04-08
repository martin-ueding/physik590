// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

#include "BootstrapDriver.hpp"
#include "parse_arguments.hpp"
#include "Settings.hpp"

#include <iostream>


/**
  Entry point for the metropolis program.

  @param argc Argument count
  @param argv Argument values
  @return Return code
  */
int main(int argc, char **argv) {
    Settings settings;

    if (parse_arguments(argc, argv, settings)) {
        return 0;
    }

    BootstrapDriver driver;
    driver.run(settings, 100);
    std::cout << std::endl;
    driver.print_results();

    return 0;
}
