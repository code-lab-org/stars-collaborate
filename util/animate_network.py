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

"""Animates a contact graph image."""

import argparse

import matplotlib.animation as anim
import matplotlib.pyplot as plt

from helper_data import read_network_log
from helper_plot import get_writer
from helper_system import check_file
from helper_system import check_not_file
from helper_system import print_progress


def main():
    """
    Animates a contact graph image.

    Notes:
        @image html raster/preview2.png width=500

    Example:
        $ python3 util/animate_map.py -h
        usage: animate_network.py [-h] [-f] [-i [IN_FILE]] [-o [OUT_FILE]] [-w]

        optional arguments:
          -h, --help            show this help message and exit
          -f, --figure          Whether to show or save
          -i [IN_FILE], --in_file [IN_FILE]
                                Path to input file
          -o [OUT_FILE], --out_file [OUT_FILE]
                                Path to output directory
          -w, --weighted        Whether data is weighted
    """

    # Input
    args = argparser()
    plt.figure(figsize=(3, 3))
    graphs = read_network_log(args.in_file)
    frames = range(len(graphs))
    if args.weighted:
        cmap = 'jet'
    else:
        cmap = 'gray'
    image = plt.imshow((graphs[0].astype(int)), cmap=cmap)
    plt.xlabel('TX')
    plt.xticks([])
    plt.ylabel('RX')
    plt.yticks([])
    plt.subplots_adjust(left=0.08, right=0.92, top=0.92, bottom=0.08)

    def update(idx):
        print_progress(idx, len(frames))
        image.set_data((graphs[idx].astype(int)))
        return (image,)

    print('Status: ')
    ani = anim.FuncAnimation(plt.gcf(), update, frames=frames, blit=True)
    if args.figure:
        plt.show()
    else:
        ani.save(args.out_file, dpi=200, writer=get_writer(10))
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
                        '--in_file',
                        default='output/network.nc4',
                        nargs='?',
                        help='Path to input file')
    parser.add_argument('-o',
                        '--out_file',
                        default='analysis/animation.mp4',
                        nargs='?',
                        help='Path to output directory')
    parser.add_argument('-w',
                        '--weighted',
                        action='store_true',
                        help='Whether data is weighted')
    args = parser.parse_args()
    check_file(args.in_file)
    check_not_file(args.out_file)
    return args


if __name__ == '__main__':
    main()
