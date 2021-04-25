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

"""Plots network node positions and connections."""

import argparse

import matplotlib.pyplot as plt

from helper_data import read_network_log
from helper_data import read_node_parameters
from helper_plot import equirectangular
from helper_plot import plot_ug
from helper_plot import plot_wg
from helper_plot import remove_margins
from helper_plot import scatter
from helper_system import check_dir
from helper_system import check_file


def main():
    """
    Plots network node positions and connections.

    Notes:
        @image html raster/map.png width=500

    Example:
        $ python3 util/plot_map.py -h
        usage: plot_map.py [-h] [-f] [-i [IN_FILE]] [-o [OUT_DIR]] [-n INDEX]
                           [-w [WGT_PATH]] [-u [UWT_PATH]]

        optional arguments:
          -h, --help            show this help message and exit
          -f, --figure          Whether to show or save
          -i [IN_FILE], --in_file [IN_FILE]
                                Path to input file
          -o [OUT_DIR], --out_dir [OUT_DIR]
                                Path to output directory
          -n INDEX, --index INDEX
                                The frame index
          -w [WGT_PATH], --wgt_path [WGT_PATH]
                                Path to weighted adjacency list log
          -u [UWT_PATH], --uwt_path [UWT_PATH]
                                Path to unweighted adjacency list log
    """
    # Input

    args = argparser()
    structure, times, _, _, colors = read_node_parameters(args.in_file)
    uwts = read_network_log(args.uwt_path)
    wgts = read_network_log(args.wgt_path)

    # Map
    plt.figure(figsize=(6.79, 3.398))
    axes = equirectangular()
    axes.stock_img()
    remove_margins()

    # Plot
    lons = structure.loc[(times[args.index]), 'longitude']
    lats = structure.loc[(times[args.index]), 'latitude']
    scatter(axes, lons, lats, colors)
    if wgts:
        plot_wg(axes, wgts[args.index], lons, lats)
    if uwts:
        plot_ug(axes, uwts[args.index], lons, lats)

    # Save
    if args.figure:
        plt.show()
    else:
        plt.savefig(args.out_dir + "map.pdf")
        plt.savefig(args.out_dir + "map.png", dpi=200)
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
    parser.add_argument('-w',
                        '--wgt_path',
                        default='',
                        nargs='?',
                        help='Path to weighted adjacency list log')
    parser.add_argument('-u',
                        '--uwt_path',
                        default='',
                        nargs='?',
                        help='Path to unweighted adjacency list log')
    args = parser.parse_args()
    check_file(args.in_file)
    check_dir(args.out_dir)
    if args.wgt_path:
        check_file(args.wgt_path)
    if args.uwt_path:
        check_file(args.uwt_path)

    return args


if __name__ == '__main__':
    main()
