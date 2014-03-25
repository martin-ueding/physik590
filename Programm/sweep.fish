#!/usr/bin/fish
# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
# Licensed under The GNU Public License Version 2 (or later)

for mass in 0.2 1.0 4.0
    for mu in 0.2 1.0 4.0
        for a in 0.02 0.1 0.4
            for rounds in 5 10
                set dir "$mass_$mu_$a_$rounds"
                echo $dir
                mkdir $dir
                pushd $dir
                ../metropolis --mass $mass --mu-squared $mu --time-step $a --rounds $rounds
                ../plotall.py
                popd
            end
        end
    end
end
