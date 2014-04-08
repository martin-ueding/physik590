// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#ifndef RESULTSET_H
#define RESULTSET_H

class ResultSet {
    public:
        BootstrappedQuantity first_moment;
        BootstrappedQuantity second_moment;
        BootstrappedQuantity energy0;
};

#endif /* end of include guard: RESULTSET_H */
