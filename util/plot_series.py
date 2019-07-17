#!/usr/bin/python3
#
# Copyright (C) 2018 Ryan Linnabary
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

"""Plots time-series data logged by simulated satellites."""

import argparse

import matplotlib.pyplot as plt
import numpy as np

from helper_data import read_node_parameters
from helper_system import check_dir
from helper_system import check_file


def main():
    """
    Plots time-series data logged by simulated satellites.

    Notes:
        @image html raster/parameters.png width=500

    Example:
        $ python3 util/plot_series.py -j
        usage: plot_series.py [-h] [-i [IN_FILE]] [-o [OUT_DIR]] [-n INDEX] [-f]

        optional arguments:
          -h, --help            show this help message and exit
          -i [IN_FILE], --in_file [IN_FILE]
                                Path to input file
          -o [OUT_DIR], --out_dir [OUT_DIR]
                                Path to output directory
          -n INDEX, --index INDEX
                                The frame index
          -f, --figure          Whether to show or save
    """

    # Input
    args = argparser()
    structure, times, nodes, _, _ = read_node_parameters(args.in_file)
    lats = structure.loc[(times, nodes[args.index]), 'latitude'].values
    lons = structure.loc[(times, nodes[args.index]), 'longitude'].values
    energies = structure.loc[(times, nodes[args.index]), 'energy'].values
    charges = structure.loc[(times, nodes[args.index]), 'charging'].values
    modes = structure.loc[(times, nodes[args.index]), 'mode'].values
    areas = structure.loc[(times, nodes[args.index]), 'area'].values
    nbors = structure.loc[(times, nodes[args.index]), 'num_neighbors'].values
    powers = np.diff(energies)
    _, axes = plt.subplots(nrows=8,
                           ncols=1,
                           sharex=True,
                           sharey=True,
                           figsize=(6.79, 5))
    plot_series(axes, lats, [], 811, r'Lat ($^\circ$)')
    plot_series(axes, lons, [], 812, r'Lon ($^\circ$)')
    plot_series(axes, areas, [], 813, r'A$_{eff}$ ($m^2$)')
    plot_series(axes, charges, [], 814, r'Charging')
    plot_series(axes, modes, [], 815, r'Mode')
    plot_series(axes, energies, [], 816, r'E ($W\cdot hr$)')
    plot_series(axes, powers, [], 817, r'P ($W$)')
    plot_series(axes, nbors, [], 818, r'Neighbors')
    plt.subplots_adjust(left=0.22, right=0.97, top=0.97, bottom=0.07)
    plt.xlabel(r'Time ($s$)')
    if args.figure:
        plt.show()
    else:
        plt.savefig('analysis/series.pdf')
        plt.savefig('analysis/series.png', dpi=300)
    plt.close()


def plot_series(axes, data, ticks, code, ylabel):
    """
    Plots charging data.

    Args:
        axes   (matplotlib.axes._subplots.AxesSubplot): Subplot
        data   (list): Time series data to plot
        ticks  (list): Ticks for x axis
        code   (str): Subplot code
        ylabel (str): Label for y axis
    """
    axes = plt.subplot(code)
    axes.set_xticks(ticks)
    axes.set_xticklabels([])
    axes.plot(data)
    axes.set_ylabel(ylabel, rotation=0)
    axes.yaxis.set_label_coords(-0.2, 0.4)
    axes.grid()


def argparser():
    """
    Obtains command-line arguments.

    Returns:
        Arguments

    Examples:
        >>> args = parser()
    """
    parser = argparse.ArgumentParser()
    parser.add_argument('-i',
                        '--in_file',
                        default='output/data.nc4',
                        nargs='?',
                        help='Path to input file')
    parser.add_argument('-o',
                        '--out_dir',
                        default='analysis/',
                        nargs='?',
                        help='Path to output directory')
    parser.add_argument('-n',
                        '--index',
                        type=int,
                        default=0,
                        help="The frame index")
    parser.add_argument('-f',
                        '--figure',
                        action='store_true',
                        help='Whether to show or save')
    args = parser.parse_args()
    check_file(args.in_file)
    check_dir(args.out_dir)
    return args


if __name__ == '__main__':
    main()
