#!/usr/bin/fish
# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>

cd _build/Release
make -j4

function common
    nice time ./metropolis --iter 30000 --inv-scatt-len 1 --gauss-width 0.055556 --corr-size 5 --corr-tau-count 30 $argv
end

common --time-step 0.05 --corr-tau-max 4
common --time-step 0.005 --corr-tau-max 4
