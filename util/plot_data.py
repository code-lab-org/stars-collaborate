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

"""Plots a single frame of geophysical data."""

import argparse

import matplotlib.pyplot as plt

from helper_data import read_data
from helper_plot import equirectangular
from helper_plot import map_blue_marble
from helper_plot import remove_margins
from helper_system import check_dir
from helper_system import check_month
from helper_system import paths_with_extension


def main():
    """
    Plots a single frame of geophysical data.

    Notes:
        @image html raster/combined.png width=500

    Example:
        $ python3 util/plot_data.py -h
        usage: plot_data.py [-h] [-f] [-i [IN_DIR]] [-o [OUT_DIR]] [-n INDEX]
                            [-m [MONTH]] [-j [IN_DIR2]]

        optional arguments:
          -h, --help            show this help message and exit
          -f, --figure          Whether to show or save
          -i [IN_DIR], --in_dir [IN_DIR]
                                Path to input directory
          -o [OUT_DIR], --out_dir [OUT_DIR]
                                Path to output directory
          -n INDEX, --index INDEX
                                The frame index
          -m [MONTH], --month [MONTH]
                                The month (2 digits)
          -j [IN_DIR2], --in_dir2 [IN_DIR2]
                                A second optional dataset
    """

    # Input
    args = argparser()
    files = paths_with_extension(args.in_dir, '.nc4')
    path = files[int(args.index)]
    colors = read_data(path, 'TAUTOT')

    # Map
    plt.figure(figsize=(14, 7.016))
    axes = equirectangular()
    map_blue_marble(axes, args.month, 'high')

    # Plot
    axes.imshow(colors, extent=(-180, 180, -90, 90), zorder=20)
    if args.in_dir2:
        files2 = paths_with_extension(args.in_dir2, '.nc4')
        path2 = files2[int(args.index)]
        colors2 = read_data(path2, 'PRECTOT')
        axes.imshow(colors2, extent=(-180, 180, -90, 90), zorder=40)
    remove_margins()

    # Save
    if args.figure:
        plt.show()
    else:
        plt.savefig(args.out_dir + "data.pdf")
        plt.savefig(args.out_dir + "data.png", dpi=100)
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
    parser.add_argument('-f',
                        '--figure',
                        action='store_true',
                        help='Whether to show or save')
    parser.add_argument('-i',
                        '--in_dir',
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
                        help="The frame index")
    parser.add_argument('-m',
                        '--month',
                        type=str,
                        default='01',
                        nargs='?',
                        help='The month (2 digits)')
    parser.add_argument('-j',
                        '--in_dir2',
                        default='',
                        nargs='?',
                        help='A second optional dataset')
    args = parser.parse_args()
    check_dir(args.out_dir)
    check_month(args.month)
    if args.in_dir2:
        check_dir(args.in_dir2)
    return args


if __name__ == '__main__':
    main()
