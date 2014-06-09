#!/usr/bin/fish
# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>

cd _build/Release
make -j4

function common
    nice time ./metropolis --iter 20000 --time-sites 30000 --corr-size 5 --corr-tau-count 20 $argv
end

#common --time-step 0.05 --corr-tau-max 4 # A989F7
#common --time-step 0.005 --corr-tau-max 4 # B006EB

common --time-step 0.05 --corr-tau-max 4 --inv-scatt-len 1 --gauss-width 0.55556
common --time-step 0.005 --corr-tau-max 4 --inv-scatt-len 1 --gauss-width 0.55556

common --time-step 0.05 --corr-tau-max 4 --inv-scatt-len -1 --gauss-width 0.055556
common --time-step 0.005 --corr-tau-max 4 --inv-scatt-len -1 --gauss-width 0.055556
