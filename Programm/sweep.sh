#!/bin/bash
# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
# Licensed under The GNU Public License Version 2 (or later)

for mass in 0.2 1.0 4.0
do
    for mu in 0.2 1.0 4.0
    do
        for a in 0.02 0.1 0.4
        do
            for rounds in 5 10
            do
                (
                dir="sweep/${mass}_${mu}_${a}_${rounds}"
                echo "$dir"
                mkdir -p "$dir"
                cd "$dir"
                ../../metropolis --mass "$mass" --mu-squared "$mu" --time-step "$a" --rounds "$rounds"
                ../../plotall.py
                ) &
            done
        done
    done
done

wait
