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
        for a in [1, 2, 3]:
            mods = ['-c', str(a)]
            futures.append(executor.submit(run, mods[:]))

        for s in [1, 2, 3]:
            mods = ['-s', str(s)]
            futures.append(executor.submit(run, mods[:]))

        for future in futures:
            future.result()

def run(mods):
    command = ['./metropolis', '-i', '300'] + mods
    print(' '.join(command))
    output = subprocess.check_output(command).decode()
    return output

if __name__ == '__main__':
    main()
