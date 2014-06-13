#!/usr/bin/python3
# -*- coding: utf-8 -*-

# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>

import argparse
import json

import prettytable

def main():
    options = _parse_args()

    table = prettytable.PrettyTable([
        'ID',
        'iter',
        'isl',
        'gauss_width',
        'corr_size',
        'time_sites',
        'time_step',
        'corr_max',
    ])

    for run in options.runs:
        with open('{run}/{run}-options.js'.format(run=run)) as f:
            info = json.load(f)

        table.add_row([
            run,
            info['iterations'],
            info['inverse_scattering_length'],
            info['gauss_width'],
            info['correlation_size'],
            info['time_sites'],
            info['time_step'],
            info['correlation_ts'][-1] * info['time_step'],
        ])

    print(table)

def _parse_args():
    """
    Parses the command line arguments.

    If the logging module is imported, set the level according to the number of
    ``-v`` given on the command line.

    :return: Namespace with arguments.
    :rtype: Namespace
    """
    parser = argparse.ArgumentParser(description="")
    parser.add_argument("runs", nargs="+", help="Run IDs")

    options = parser.parse_args()

    return options

if __name__ == "__main__":
    main()
