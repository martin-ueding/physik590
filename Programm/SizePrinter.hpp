// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

/**
  Pretty prints file sizes.
  */
class SizePrinter {
    public:
        /**
          Formats the size into a string.

          @param size File size in bytes
          @return Formatted string
          */
        std::string format(size_t size);

        /**
          Prints the formatted output.

          @param size File size in bytes
          */
        void print(size_t size);

    private:
        /**
          List of binary suffixes.
          */
        std::vector<std::string> suffixes {"", "kiB", "MiB", "GiB", "TiB"};

        /**
          Finds the suffix level.

          @param size File size in bytes
          @return Suffix level
          */
        int find_level(size_t size);
};
