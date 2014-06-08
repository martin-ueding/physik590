#!/usr/bin/python3
# -*- coding: utf-8 -*-

# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
# Licensed under The GNU Public License Version 2 (or later)

import argparse
import datetime
import glob
import json
import os.path
import re

import scipy.optimize as op
import matplotlib.pyplot as pl
import numpy as np
import unitprint
import prettytable

def main():
    options = _parse_args()

    energies = {}

    if len(options.prefixes) == 0:
        dirnames = sorted(glob.glob('*/'))
    else:
        dirnames = sorted([
            dirname
            for prefix in options.prefixes
            for dirname in glob.glob('{}*/'.format(prefix))
        ])

    for dirname in dirnames:
        run = os.path.basename(os.path.dirname(dirname))
        print('Run:', run)
        options_filename = get_filename(run, '-options.js')
        parameters = {}
        if os.path.isfile(options_filename):
            with open(options_filename) as f:
                parameters = json.load(f)
        else:
            print(options_filename, 'does not exist.')

        plotted = []

        print('From virial theorem:')
        E_0 = find_E_0(run)

        print('From C_11(t):')
        plot_correlations(dirname, '*-c11.csv', E_0)

        print('From eigenvalues:')
        result = fit_eigenvalues(dirname, '*-eigenvalue-??.csv', E_0)

        isl = parameters['inverse_scattering_length']
        if not isl in energies:
            energies[isl] = {}
        for n, energy in result.items():
            if not n in energies[isl]:
                energies[isl][n] = []
            energies[isl][n].append([parameters['gauss_width']] + list(energy))

        print()

    print(json.dumps(energies, sort_keys=True, indent=4))

    for isl, isl_data in sorted(energies.items()):
        for n, n_data in sorted(isl_data.items()):
            print('inverse_scattering_length', isl, 'n', n)
            data = np.array(sorted(n_data))
            print(data)
            np.savetxt(os.path.join('isl{}-n{}.csv'.format(isl, n)), data)

    for isl in energies.keys():
        files = glob.glob('isl{}-*.csv'.format(isl))
        files.sort()
        n = 0
        n_max = len(files)
        even_fig = pl.figure()
        even_ax = even_fig.add_subplot(1, 1, 1)
        odd_fig = pl.figure()
        odd_ax = odd_fig.add_subplot(1, 1, 1)
        for file_ in files:
            n += 1
            if n > 8:
                continue
            try:
                print(file_)
                data = np.loadtxt(file_)
                ax = even_ax if n % 2 == 0 else odd_ax
                ax.errorbar(data[:, 0], data[:, 1], yerr=data[:, 2], label=os.path.basename(file_), color=str((n-1)/n_max))
            except IndexError as e:
                print(e)
        even_ax.grid(True)
        even_ax.legend(loc='best', prop={'size': 6})
        even_fig.savefig('isl{}-even.pdf'.format(isl))
        odd_ax.grid(True)
        odd_ax.legend(loc='best', prop={'size': 6})
        odd_fig.savefig('isl{}-odd.pdf'.format(isl))

def get_filename(run, ending):
    '''
    Builds up a filename.

    :param str run: ID of the run
    :param str ending: Suffix of the filename, just as in the C++ program
    :returns: Complete filename
    :rtype: str
    '''
    return os.path.join(run, run+ending)

def find_E_0(run):
    pattern = '-c11.csv'
    csv_file = get_filename(run, pattern)
    data = np.loadtxt(csv_file)
    t = data[0, 0]
    corr_val = data[0, 1]
    corr_err = data[0, 2]

    print('E_0 =', unitprint.siunitx(corr_val, corr_err))

    return corr_val, corr_err
        

def fit_eigenvalues(dirname, pattern, E_0):
    results = {}
    table = prettytable.PrettyTable(['n', 'E_n - E_0', 'E_n'])
    for csv_file in sorted(glob.glob(os.path.join(dirname, pattern))):
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
            if needs_plotting(csv_file):
                pl.savefig(csv_file.replace('.csv', '.pdf'))
                np.savetxt(csv_file.replace('.csv', '-fit.csv'), np.column_stack([fx, fy]))
            pl.clf()



            E_n_val = popt[0] + E_0[0]
            E_n_err = np.sqrt(d[0]**2 + E_0[1]**2)
            table.add_row([
                number,
                unitprint.siunitx(popt[0], d[0], error_digits=2),
                unitprint.siunitx(E_n_val, E_n_err, error_digits=2),
            ])


            results[number] = E_n_val, E_n_err
        except RuntimeError as e:
            print(e)
        except AttributeError as e:
            # This error probably occurs when the ``pconv`` matrix is just
            # ``nan``.
            print(e)

    print(table)

    return results

def plot_correlations(dirname, pattern, E0):
    for filename in glob.glob(os.path.join(dirname, pattern)):
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

        print('E_1 - E_0 =', unitprint.siunitx(popt[0], np.sqrt(pconv.diagonal()[0]), error_digits=2))
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
        if needs_plotting(filename):
            pl.savefig(filename.replace('.csv', '.pdf'))
        pl.clf()

def needs_plotting(filename):
    '''
    Determines whether a file needs to be replotted.

    This is the case if the ``.pdf`` file is older than the corresponding
    ``.csv`` file.

    :param str filename: Name or path of the ``.csv`` file.
    '''
    csv_time = datetime.datetime.fromtimestamp(os.path.getmtime(filename))
    pdf_file = filename.replace('.csv', '.pdf')
    if not os.path.isfile(pdf_file):
        return True
    pdf_time = datetime.datetime.fromtimestamp(os.path.getmtime(pdf_file))
    return csv_time > pdf_time

def decay(x, tau, ampl):
    '''
    Exponential decay fit function.
    '''
    return ampl * np.exp(-x / tau)

def time_evolution(x, e_n, ampl):
    '''
    Time evolution fit function.
    '''
    return ampl * np.exp(-e_n * x)

def time_evolution_with_offset(x, e_n, ampl, offset):
    '''
    Time evolution with constant offset fit function.
    '''
    return time_evolution(x, e_n, ampl) + offset

def decay_with_offset(x, tau, ampl, offset):
    '''
    Exponential decay with constant offset fit function.
    '''
    return decay(x, tau, ampl) + offset

def _parse_args():
    """
    Parses the command line arguments.

    :return: Namespace with arguments.
    :rtype: Namespace
    """
    parser = argparse.ArgumentParser(description="")
    parser.add_argument("prefixes", nargs="*", help="Runs to analyze")

    options = parser.parse_args()

    return options

if __name__ == "__main__":
    main()
