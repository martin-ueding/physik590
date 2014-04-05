#!/usr/bin/python3
# -*- coding: utf-8 -*-

# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
# Licensed under The GNU Public License Version 2 (or later)

import argparse
import matplotlib.pyplot as pl
import numpy as np
import glob
import os.path
import datetime

def main():
    options = _parse_args()

    plotted = []

    for csv_file in glob.glob('out/histogram-position-*.csv'):
        if csv_file in plotted:
            continue
        plotted.append(csv_file)
        if needs_plotting(csv_file):
            print('Plotting', csv_file)
            plot_histogram(csv_file)

    for csv_file in glob.glob('out/trajectory-*.csv'):
        if csv_file in plotted:
            continue
        plotted.append(csv_file)
        if needs_plotting(csv_file):
            print('Plotting', csv_file)
            auto_plot_trajectory(csv_file)

    for csv_file in glob.glob('out/histogram-*.csv'):
        if csv_file in plotted:
            continue
        plotted.append(csv_file)
        if needs_plotting(csv_file):
            print('Plotting', csv_file)
            auto_plot_histogram(csv_file)

    plot_combined_histogram('out/histogram-position-*.csv')
    plot_combined_histogram('out/histogram-action-*.csv')

def needs_plotting(filename):
    csv_time = datetime.datetime.fromtimestamp(os.path.getmtime(filename))
    pdf_file = filename.replace('.csv', '.pdf')
    if not os.path.isfile(pdf_file):
        return True
    pdf_time = datetime.datetime.fromtimestamp(os.path.getmtime(pdf_file))
    return csv_time > pdf_time

def insert_theory(ax):
    x = np.linspace(-4, 4, 1000)
    y = 0.59 * np.exp(- 1.1 * x**2)
    ax.plot(x, y, label='Theorie', linestyle='dashed')

def plot_combined_histogram(wildcard):
    print('Plotting', wildcard)
    filenames = sorted(glob.glob(wildcard))

    fig = pl.figure()
    ax = fig.add_subplot(1, 1, 1)

    for filename in filenames:
        data = np.genfromtxt(filename)
        bins = data[:, 0]
        counts = data[:, 1]
        width = bins[3] - bins[2]
        counts /= width
        selection = abs(bins) < 4
        ax.plot(bins[selection], counts[selection], label=filename)

    insert_theory(ax)

    ax.set_title(wildcard)
    ax.set_xlabel(r'Messgröße')
    ax.set_ylabel(r'relative Häufigkeit')
    ax.grid(True)
    ax.legend(loc='best', prop={'size': 6})

    fig.savefig(wildcard.replace('.csv', '.pdf'))


def plot_histogram(filename):
    data = np.genfromtxt(filename)

    fig = pl.figure()
    ax = fig.add_subplot(1, 1, 1)
    bins = data[:, 0]
    counts = data[:, 1]
    width = bins[3] - bins[2]
    selection = abs(bins) < 4

    counts /= width

    ax.plot(bins[selection], counts[selection], marker='+', linestyle='none', label='Metropolis')
    ax.set_title(filename)
    ax.set_xlabel(r'Position $x$')
    ax.set_ylabel(r'relative Häufigkeit')
    ax.grid(True)

    insert_theory(ax)

    ax.legend(loc='best')


    fig.savefig(filename.replace('.csv', '.pdf'))

def auto_plot_histogram(filename):
    data = np.genfromtxt(filename)

    fig = pl.figure()
    ax = fig.add_subplot(1, 1, 1)
    bins = data[:, 0]
    counts = data[:, 1]
    width = bins[3] - bins[2]
    counts /= width

    ax.plot(bins, counts, marker='+', linestyle='none')
    ax.set_title(filename)
    ax.set_xlabel(r'Messgröße')
    ax.set_ylabel(r'relative Häufigkeit')
    ax.grid(True)

    fig.savefig(filename.replace('.csv', '.pdf'))

def auto_plot_trajectory(filename):
    data = np.genfromtxt(filename)

    fig = pl.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.plot(data[:, 0], data[:, 1])
    ax.set_title(filename)
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

    return options

if __name__ == "__main__":
    main()
