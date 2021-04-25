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

"""Plots a contact graph image."""

import argparse

import matplotlib.pyplot as plt
import numpy as np

from helper_data import read_network_log
from helper_system import check_dir
from helper_system import check_file


def main():
    """
    Plots a contact graph image.

    Notes:
        @image html raster/unweighted.png width=300

    Example:
        $ python3 util/plot_map.py -h
        usage: plot_network.py [-h] [-i [IN_FILE]] [-o [OUT_DIR]] [-n INDEX]
                               [-w] [-f]

        optional arguments:
          -h, --help            show this help message and exit
          -i [IN_FILE], --in_file [IN_FILE]
                                Path to input file
          -o [OUT_DIR], --out_dir [OUT_DIR]
                                Path to output directory
          -n INDEX, --index INDEX
                                The frame index
          -w, --weighted        Whether data is weighted
          -f, --figure          Whether to show or save
    """

    # Input
    args = argparser()
    plt.figure(figsize=(3, 3))
    graphs = read_network_log(args.in_file)
    graph = np.array(graphs[args.index])
    if args.weighted:
        cmap = 'jet'
        out_path = args.out_dir + "weighted"
    else:
        cmap = 'gray'
        out_path = args.out_dir + "unweighted"
    plt.imshow(np.invert(graph.astype(int)), cmap=cmap)
    plt.xlabel('TX')
    plt.xticks([])
    plt.ylabel('RX')
    plt.yticks([])
    plt.subplots_adjust(left=0.08, right=0.92, top=0.92, bottom=0.08)
    if args.figure:
        plt.show()
    else:
        plt.savefig(out_path + ".pdf")
        plt.savefig(out_path + ".png", dpi=300)
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
                        '--in_file',
                        default='output/network.nc4',
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
    parser.add_argument('-w',
                        '--weighted',
                        action='store_true',
                        help='Whether data is weighted')
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
