#!/usr/bin/python3
# -*- coding: utf-8 -*-

# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
# Licensed under The GNU Public License Version 2 (or later)

import argparse
import matplotlib.pyplot as pl
import numpy as np
import glob
import os.path
import scipy.optimize as op
import datetime

def main():
    options = _parse_args()

    runs = sorted(glob.glob('out/?????????*/'))

    for run in runs:
        print('Run:', run)

        plotted = []

        plot_with(auto_plot_histogram, 'histogram-position-*.csv', run, plotted)
        plot_with(auto_plot_trajectory, 'trajectory-*.csv', run, plotted)
        plot_with(auto_plot_histogram, 'histogram-*.csv', run, plotted)

        plot_correlations(run, '*-correlations.csv')

        fit_eigenvalues(run, '*-eigenvalue-*.csv')

def fit_eigenvalues(run, pattern):
    for csv_file in sorted(glob.glob(os.path.join(run, pattern))):
        data = np.loadtxt(csv_file)
        if len(data) == 0:
            continue
        tau = data[:, 0]
        y_val = data[:, 1]
        y_err = data[:, 2]

        selection = tau < 4

        tau_s = tau[selection]
        y_val_s = y_val[selection]

        popt, pconv = op.curve_fit(time_evolution_with_offset, tau_s, y_val_s)

        fx = np.linspace(min(tau_s), max(tau_s), 1000)
        fy = time_evolution_with_offset(fx, *popt)

        pl.clf()
        pl.plot(tau_s, y_val_s, linestyle='none', marker='+')
        pl.plot(fx, fy)
        pl.savefig(csv_file.replace('.csv', '.pdf'))
        pl.clf()

        print(csv_file, popt[0])

def plot_with(function, pattern, run, plotted):
    for csv_file in glob.glob(os.path.join(run, pattern)):
        if csv_file in plotted:
            continue
        plotted.append(csv_file)
        if needs_plotting(csv_file):
            print('Plotting', csv_file)
            function(csv_file)

def decay(x, tau, ampl):
    return ampl * np.exp(-x / tau)

def time_evolution(x, e_n, ampl):
    return ampl * np.exp(-e_n * x)

def time_evolution_with_offset(x, e_n, ampl, offset):
    return time_evolution(x, e_n, ampl) + offset

def decay_with_offset(x, tau, ampl, offset):
    return decay(x, tau, ampl) + offset

def plot_correlations(run, pattern):
    for filename in glob.glob(os.path.join(run, pattern)):
        data = np.loadtxt(filename)
        t = data[:, 0] * 0.1
        corr_val = data[:, 1]
        corr_err = data[:, 2]

        cutoff = 40
        cutoff2 = cutoff * 2.5

        popt, pconv = op.curve_fit(decay, t[:cutoff], corr_val[:cutoff], sigma=corr_err[:cutoff])

        #print('popt:', popt)
        #print(pconv)

        E1_val = popt[0]
        E1_err = np.sqrt(pconv.diagonal()[0])

        print('E1 - E0 = {} +- {}'.format(E1_val, E1_err))

        x = np.linspace(np.min(t[:cutoff2]), np.max(t[:cutoff2]), 100)
        y = decay(x, *popt)

        pl.clf()
        pl.errorbar(t[:cutoff2], corr_val[:cutoff2], yerr=corr_err[:cutoff2])
        pl.plot(x, y)
        pl.grid(True)
        pl.title('Korrelationen')
        pl.xlabel(r'$\Delta\tau$')
        pl.ylabel(r'$\langle x(\tau) x(\tau + \Delta \tau) \rangle$')
        #pl.yscale('log')
        pl.savefig(filename.replace('.csv', '.pdf'))
        pl.clf()

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
    ax.plot(x, y, label='Theorie diskret', linestyle='dashed', color='black')
    y = 1/np.sqrt(np.pi) * np.exp(- x**2)
    ax.plot(x, y, label='Theorie kontinuierlich', linestyle='dotted', color='black')

def plot_histogram(filename):
    data = np.genfromtxt(filename)

    fig = pl.figure()
    ax = fig.add_subplot(1, 1, 1)
    bins = data[:, 0]
    counts = data[:, 1]
    width = bins[3] - bins[2]
    selection = abs(bins) < 4

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
