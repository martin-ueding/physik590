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
        for mass in [0.2, 1.0, 4.0]:
            for mu in [0.2, 1.0, 4.0]:
                for a in [0.02, 0.1, 0.4]:
                    for rounds in [5, 10]:
                        futures.append(executor.submit(run, mass, mu, a, rounds))

        for future in futures:
            print(future.result())

def run(mass, mu, a, rounds):
    dirname = 'sweep/{}_{}_{}_{}'.format(mass, mu, a, rounds)
    os.makedirs(dirname, exist_ok=True)
    os.chdir(dirname)
    command = [
        '../../metropolis',
        '--mass', str(mass),
        '--mu-squared', str(mu),
        '--time-step', str(a),
        '--rounds', str(rounds),
    ]
    output = subprocess.check_output(command).decode()
    output += '\n'
    output += subprocess.check_output(['../../plotall.py']).decode()
    os.chdir('../..')

    return output

if __name__ == '__main__':
    main()
