// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <string>

class Histogram {
    public:
        /**
          Prints the current histogram contents to the standard output.
          */
        void print();

        /**
          Saves the current histogram to the given file.
          */
        void save(std::string filename);

        /**
          Maximum size for a single histogram.
          */
        const size_t MAX_CACHE_ENTRIES = 1024L * 1024 * 1024 / sizeof(double);

        /**
          Push a new value into the histogram.
          **/
        virtual void push(double value) = 0;

        /**
          Writes the current histogram into the given stream.

          @param outfile Stream to write to
          */
        virtual void write_histogram(std::ostream &outfile) = 0;
};

#endif /* end of include guard: HISTOGRAM_H */
