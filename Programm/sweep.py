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
        for X in [1, 2, 3]:
            futures.append(executor.submit(run, ['-c', str(X)]))

        for X in [1, 2, 3]:
            futures.append(executor.submit(run, ['-s', str(X)]))

        for X in [300, 1000, 3000]:
            futures.append(executor.submit(run, ['-i', str(X), '-b', str(X // 3)]))

        for future in futures:
            future.result()

def run(mods):
    command = ['./metropolis'] + mods
    print(' '.join(command))
    output = subprocess.check_output(command).decode()
    return output

if __name__ == '__main__':
    main()
