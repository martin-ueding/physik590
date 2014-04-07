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
        for X in [0.6, 0.4, 0.2, 1.0]:
            p1 = ['--margin', str(X)]
            for X in [1000, 500, 2000]:
                p2 = ['-t', str(X)]
                for X in [1, 2, 3]:
                    p3 = ['-s', str(X)]
                    for X in [1, 2, 3]:
                        p4 = ['-c', str(X)]
                        for X in [0.1]:
                            p5 = ['--margin', str(X)]
                            for X in [0.1, 0.2, 0.05]:
                                p5 = ['-a', str(X)]
                                for X in [300, 1000]:
                                    p6 = ['-i', str(X)]

                                    mods = p1 + p2 + p3 + p4 + p5 + p6
                                    futures.append(executor.submit(run, mods[:]))

        for future in futures:
            future.result()

def run(mods):
    command = ['./metropolis'] + mods
    print(' '.join(command))
    output = subprocess.check_output(command).decode()
    return output

if __name__ == '__main__':
    main()
