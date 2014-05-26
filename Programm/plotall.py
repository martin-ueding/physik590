#!/usr/bin/python3
# -*- coding: utf-8 -*-

# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
# Licensed under The GNU Public License Version 2 (or later)

import argparse
import datetime
import glob
import os.path
import re

import scipy.optimize as op
import matplotlib.pyplot as pl
import numpy as np
import unitprint

def main():
    options = _parse_args()

    if len(options.prefixes) == 0:
        runs = sorted(glob.glob('out/*/'))
    else:
        runs = sorted([
            run
            for prefix in options.prefixes
            for run in glob.glob('out/{}*/'.format(prefix))
        ])

    for run in runs:
        print('Run:', os.path.basename(os.path.dirname(run)))

        plotted = []

        plot_with(auto_plot_histogram, 'histogram-position-*.csv', run, plotted)
        plot_with(auto_plot_trajectory, 'trajectory-*.csv', run, plotted)
        plot_with(auto_plot_histogram, 'histogram-*.csv', run, plotted)

        print('From virial theorem:')
        E_0 = find_E_0(run)

        print('From C_11(t):')
        plot_correlations(run, '*-c11.csv', E_0)

        print('From eigenvalues:')
        fit_eigenvalues(run, '*-eigenvalue-*.csv', E_0)

def find_E_0(run):
    pattern = '*-c11.csv'
    csv_file = glob.glob(os.path.join(run, pattern))[0]
    data = np.loadtxt(csv_file)
    t = data[0, 0]
    corr_val = data[0, 1]
    corr_err = data[0, 2]

    print('E_0 =', unitprint.siunitx(corr_val, corr_err))

    return corr_val, corr_err
        

def fit_eigenvalues(run, pattern, E_0):
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
        y_err_s = y_err[selection]

        number = re.search(r'-(\d+).csv', csv_file).group(1)

        func = time_evolution
        try:
            popt, pconv = op.curve_fit(func, tau_s, y_val_s, sigma=y_err_s)
            d = np.sqrt(pconv.diagonal())

            fx = np.linspace(min(tau_s), max(tau_s), 1000)
            fy = func(fx, *popt)

            pl.clf()
            pl.errorbar(tau_s, y_val_s, yerr=y_err_s, linestyle='none', marker='+')
            pl.plot(fx, fy)
            pl.grid(True)
            pl.title(os.path.basename(csv_file))
            pl.xlabel('imaginäre Zeit')
            pl.ylabel('Eigenwert')
            pl.yscale('log')
            pl.savefig(csv_file.replace('.csv', '.pdf'))
            pl.clf()

            E_n_val = popt[0] + E_0[0]
            E_n_err = np.sqrt(d[0]**2 + E_0[1]**2)

            print('E_{} ='.format(number), unitprint.siunitx(E_n_val, E_n_err, error_digits=2))
        except RuntimeError as e:
            print(e)

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

def plot_correlations(run, pattern, E0):
    for filename in glob.glob(os.path.join(run, pattern)):
        data = np.loadtxt(filename)
        t = data[:, 0]
        corr_val = data[:, 1]
        corr_err = data[:, 2]

        cutoff = 4
        selection = t < cutoff

        popt, pconv = op.curve_fit(time_evolution, t[selection], corr_val[selection], sigma=corr_err[selection])

        E1_val = popt[0]
        E1_err = np.sqrt(pconv.diagonal()[0])

        E1_val += E0[0]
        E1_err += np.sqrt(E1_err**2 + E0[1]**2)

        print('E_1 =', unitprint.siunitx(E1_val, E1_err, error_digits=2))

        x = np.linspace(np.min(t[selection]), np.max(t[selection]), 100)
        y = time_evolution(x, *popt)

        pl.clf()
        pl.errorbar(t[selection], corr_val[selection], yerr=corr_err[selection])
        pl.plot(x, y)
        pl.grid(True)
        pl.title(os.path.basename(filename))
        pl.xlabel(r'$\Delta\tau$')
        pl.ylabel(r'$\langle x(\tau) x(\tau + \Delta \tau) \rangle$')
        pl.yscale('log')
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
    parser.add_argument("prefixes", nargs="*", help="Positional arguments.")

    options = parser.parse_args()

    return options

if __name__ == "__main__":
    main()
