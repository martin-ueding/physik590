#!/usr/bin/fish
# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>

cd _build/Release
make -j4

./metropolis --time-step 0.005 --iter 30000 --inv-scatt-len 1 --gauss-width 0.055556 --corr-size 5
