// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef SIZEPRINT_H
#define SIZEPRINT_H

#include <string>
#include <vector>

class SizePrinter {
    public:
        std::string format(unsigned int size);
        void print(unsigned int size);
        int find_level(unsigned int size);
    private:
        std::vector<std::string> suffixes{"", "kiB", "MiB", "GiB", "TiB"};
};

#endif /* end of include guard: SIZEPRINT_H */
