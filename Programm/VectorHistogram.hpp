// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef VECTORHISTOGRAM_H
#define VECTORHISTOGRAM_H

#include <string>

class VectorHistogram {
    public:
        VectorHistogram(size_t bins);

        /**
          Push a new value into the histogram.
          **/
        void push(double value);

        /**
          Writes the current histogram into the given stream.

          @param outfile Stream to write to
          */
        void write_histogram(std::ostream &outfile);
};

#endif /* end of include guard: VECTORHISTOGRAM_H */
