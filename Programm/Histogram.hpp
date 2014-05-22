// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include <boost/archive/text_oarchive.hpp>

#include <string>
#include <vector>

class Histogram {
        friend class boost::serialization::access;

    public:
        Histogram();

        Histogram(double min, double max, size_t bins);

        Histogram &operator+=(Histogram &other);

        /**
          Push a new value into the histogram.
          **/
        void operator()(double value);

        double operator[](size_t i) {
            return static_cast<double>(bins[i]) / points_pushed;
        }

        /**
          Saves the current histogram to the given file.
          */
        void save(std::string &filename);

        double get_min() {
            return min;
        }

        double get_max() {
            return max;
        }

        unsigned size() {
            return bins.size();
        }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar &min;
            ar &max;
            ar &bins;
            ar &points_pushed;
        }
#pragma clang diagnostic pop

    protected:
        double min;
        double max;
        std::vector<int> bins;
        int points_pushed {0};

        size_t map_bin(double value);
};
