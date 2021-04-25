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

"""Animates a geophysical data-set."""

import argparse

import matplotlib.animation as anim
import matplotlib.pyplot as plt

from helper_data import read_data
from helper_plot import equirectangular
from helper_plot import get_writer
from helper_plot import map_blue_marble
from helper_plot import plot_data
from helper_plot import remove_margins
from helper_system import check_dir
from helper_system import check_month
from helper_system import paths_with_extension
from helper_system import print_progress


def main():
    """
    Animates a geophysical data-set.

    Notes:
        @image html raster/preview1.png width=500

    Example:
        $ python3 util/animate_data.py -h
        usage: animate_data.py [-h] [-f] [-i [IN_DIR]] [-o [OUT_DIR]]
                               [-m [MONTH]] [-v [VARIABLE]]

        optional arguments:
          -h, --help            show this help message and exit
          -f, --figure          Whether to show or save
          -i [IN_DIR], --in_dir [IN_DIR]
                                Path to input directory
          -o [OUT_DIR], --out_dir [OUT_DIR]
                                Path to output directory
          -m [MONTH], --month [MONTH]
                                The month (2 digits)
          -v [VARIABLE], --variable [VARIABLE]
                                The netcdf variable name
    """

    # Inputs
    args = argparser()
    files = paths_with_extension(args.in_dir, '.nc4')
    frames = range(0, len(files))

    # Map
    plt.figure(figsize=(14, 7.016))
    axes = equirectangular()
    map_blue_marble(axes, args.month, 'high')
    image = plot_data(axes, read_data(files[0], args.variable))
    remove_margins()

    # Update
    def update(idx):
        print_progress(idx, len(files))
        image.set_data(read_data(files[idx], args.variable))
        return (image,)

    # Animate
    print('Status: ')
    ani = anim.FuncAnimation(plt.gcf(), update, frames=frames, blit=True)
    if args.figure:
        plt.show()
    else:
        ani.save(args.out_dir + 'animation.mp4', dpi=150, writer=get_writer(1))
    plt.close()
    print()


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
    parser.add_argument('-m',
                        '--month',
                        type=str,
                        default='01',
                        nargs='?',
                        help='The month (2 digits)')
    parser.add_argument('-v',
                        '--variable',
                        type=str,
                        default='TAUTOT',
                        nargs='?',
                        help='The netcdf variable name')
    args = parser.parse_args()
    check_dir(args.in_dir)
    check_dir(args.out_dir)
    check_month(args.month)
    return args


if __name__ == '__main__':
    main()
