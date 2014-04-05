#!/usr/bin/python3
# -*- coding: utf-8 -*-

# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
# Licensed under The GNU Public License Version 2 (or later)

import concurrent.futures
import os
import subprocess

def main():
    with concurrent.futures.ProcessPoolExecutor(4) as executor:
        futures = []
        for mu in [1, 1e10, 1e20]:
            for a in [1, 0.1, 0.01]:
                for iterations in [1000, 10000]:
                    futures.append(executor.submit(run, mu, a, iterations))

        for future in futures:
            print(future.result())

def run(mu, a, iterations):
    command = [
        './metropolis',
        '--mu-squared', str(mu),
        '--time-step', str(a),
        '--iterations', str(iterations),
    ]
    output = subprocess.check_output(command).decode()
    return output

if __name__ == '__main__':
    main()
