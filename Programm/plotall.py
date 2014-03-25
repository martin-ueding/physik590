#!/usr/bin/python3
# -*- coding: utf-8 -*-

# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>

import argparse
import matplotlib.pyplot as pl
import numpy as np
import glob

def main():
    options = _parse_args()

    for csv_file in glob.glob('trajectory-*.csv'):
        auto_plot_trajectory(csv_file)

    for csv_file in glob.glob('histogram-*.csv'):
        auto_plot_histogram(csv_file)

def auto_plot_histogram(filename):
    data = np.genfromtxt(filename)

    fig = pl.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.plot(data[:, 0], data[:, 1], marker='o')
    ax.set_xlabel(r'relative Häufigkeit')
    ax.set_ylabel(r'Position $x$')
    ax.grid(True)
    fig.savefig(filename.replace('.csv', '.pdf'))

def auto_plot_trajectory(filename):
    data = np.genfromtxt(filename)

    fig = pl.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.plot(data[:, 0], data[:, 1], marker='o')
    ax.set_xlabel(r'Zeit $j$')
    ax.set_ylabel(r'Position $x_j$')
    ax.grid(True)
    fig.savefig(filename.replace('.csv', '.pdf'))

def _parse_args():
    """
    Parses the command line arguments.

    If the logging module is imported, set the level according to the number of
    ``-v`` given on the command line.

    :return: Namespace with arguments.
    :rtype: Namespace
    """
    parser = argparse.ArgumentParser(description="")
    #parser.add_argument("args", metavar="N", type=str, nargs="*", help="Positional arguments.")
    #parser.add_argument("", dest="", type="", default=, help=)
    #parser.add_argument("--version", action="version", version="<the version>")
    parser.add_argument("-v", dest='verbose', action="count", help='Enable verbose output. Can be supplied multiple times for even more verbosity.')

    options = parser.parse_args()

    # Try to set the logging level in case the logging module is imported.
    try:
        if options.verbose == 1:
            logging.basicConfig(level=logging.INFO)
        elif options.verbose == 2:
            logging.basicConfig(level=logging.DEBUG)
    except NameError as e:
        pass

    return options

if __name__ == "__main__":
    main()
