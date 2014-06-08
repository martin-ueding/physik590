#!/usr/bin/fish
# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>

cd _build/Release
make -j4

function common
    nice time ./metropolis --iter 17000 --time-sites 30000 --inv-scatt-len 1 --gauss-width 0.055556 --corr-size 5 --corr-tau-count 20 $argv
end

common --time-step 0.05 --corr-tau-max 4 # E8C8DD
common --time-step 0.005 --corr-tau-max 4 # 1654F1
