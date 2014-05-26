#!/usr/bin/fish
# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
# Licensed under The GNU Public License Version 2 (or later)

# This file is from 2014-05-26. It will run over night and compute data points for a few different values of the inverse scattering length.

cd _build/Release

make -j4

set common --iter 20000 --samples 10000 --corr-tau-max 6 --corr-size 6

time nice ./metropolis $common --inv-scatt-len 1 --gauss-width 1
time nice ./metropolis $common --inv-scatt-len 1 --gauss-width 0.5
time nice ./metropolis $common --inv-scatt-len 1 --gauss-width 0.1
time nice ./metropolis $common --inv-scatt-len 1 --gauss-width 0.01

time nice ./metropolis $common --inv-scatt-len -1 --gauss-width 1
time nice ./metropolis $common --inv-scatt-len -1 --gauss-width 0.5
time nice ./metropolis $common --inv-scatt-len -1 --gauss-width 0.1
time nice ./metropolis $common --inv-scatt-len -1 --gauss-width 0.01
