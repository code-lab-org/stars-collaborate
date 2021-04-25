#!/usr/bin/python3
#
# Copyright (C) 2018 The Ohio State University
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

"""Plots a measurement map and graph."""

import argparse

import cartopy.crs as ccrs
import matplotlib.pyplot as plt
import numpy as np

from helper_data import read_data
from helper_data import read_measurement
from helper_plot import map_blue_marble
from helper_plot import remove_margins
from helper_plot import scatter_small_red
from helper_system import check_dir
from helper_system import only_name
from helper_system import paths_which_contain
from helper_system import paths_with_extension


def main():
    """
    Plots a measurement map and graph.

    Notes:
        @image html raster/measurement.png width=500

    Example:
        $ python3 util/book_measurement.py -h
        usage: plot_measurement.py [-h] [-i [IN_DIR]] [-d [DATA_DIR]]
                                   [-o [OUT_DIR]] [-n INDEX]

        optional arguments:
          -h, --help            show this help message and exit
          -i [IN_DIR], --in_dir [IN_DIR]
                                Path to input directory
          -d [DATA_DIR], --data_dir [DATA_DIR]
                                Path to input directory
          -o [OUT_DIR], --out_dir [OUT_DIR]
                                Path to output directory
          -n INDEX, --index INDEX
                                The file index
    """

    # Input
    args = argparser()
    data_files = paths_with_extension(args.data_dir, '.nc4')
    paths1 = sorted(paths_which_contain(args.in_dir, "PRECTOT"))
    paths2 = sorted(paths_which_contain(args.in_dir, "TAUTOT"))
    paths2 = [p for p in paths2 if '_N00000_' not in p]
    paths = paths1 + paths2
    for path in paths:
        log, index, _ = read_measurement(path)

        # Graph
        plt.figure(figsize=(9, 3))
        plt.plot(log['measurement'])
        plt.ylabel('Measurement')
        plt.xlabel('Time')
        plt.xticks([])
        plt.grid()
        plt.tight_layout()
        out = f'{args.out_dir}{only_name(path)}_graph'
        plt.savefig(f'{out}.png', dpi=300)
        plt.close()


def argparser():
    """
    Obtains command-line arguments.

    Returns:
        Arguments

    Examples:
        >>> args = argparser()
    """
    parser = argparse.ArgumentParser()
    parser.add_argument('-i',
                        '--in_dir',
                        default='output/',
                        nargs='?',
                        help='Path to input directory')
    parser.add_argument('-d',
                        '--data_dir',
                        default='input/nc4/',
                        nargs='?',
                        help='Path to input directory')
    parser.add_argument('-o',
                        '--out_dir',
                        default='analysis/',
                        nargs='?',
                        help='Path to output directory')
    parser.add_argument('-n',
                        '--index',
                        type=int,
                        default=0,
                        help="The file index")
    args = parser.parse_args()
    check_dir(args.in_dir)
    check_dir(args.out_dir)
    return args


if __name__ == '__main__':
    main()
