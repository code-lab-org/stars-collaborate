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

"""Animates network node positions and connections."""

import argparse

import matplotlib.animation as anim
import matplotlib.pyplot as plt
import numpy as np

from helper_data import read_network_log
from helper_data import read_node_parameters
from helper_plot import equirectangular
from helper_plot import figure_title_bar
from helper_plot import get_writer
from helper_plot import map_gray_land
from helper_plot import plot_ug
from helper_plot import plot_wg
from helper_plot import scatter
from helper_plot import write_clock
from helper_plot import write_title
from helper_system import check_dir
from helper_system import check_file
from helper_system import print_progress


def main():
    """
    Animates network node positions and connections.

    Notes:
        @image html raster/preview0.png width=500

    Example:
        $ python3 util/animate_map.py -h
        usage: animate_map.py [-h] [-f] [-i [IN_FILE]] [-o [OUT_DIR]]
                              [-d [DATA_DIR]] [-t [TITLE]] [-w [WGT_PATH]]
                              [-u [UWT_PATH]]

        optional arguments:
          -h, --help            show this help message and exit
          -f, --figure          Whether to show or save
          -i [IN_FILE], --in_file [IN_FILE]
                                Path to input file
          -o [OUT_DIR], --out_dir [OUT_DIR]
                                Path to output directory
          -d [DATA_DIR], --data_dir [DATA_DIR]
                                Path to data directory
          -t [TITLE], --title [TITLE]
                                Title for the plot
          -w [WGT_PATH], --wgt_path [WGT_PATH]
                                Path to weighted adjacency list log
          -u [UWT_PATH], --uwt_path [UWT_PATH]
                                Path to unweighted adjacency list log
    """

    # Input
    args = argparser()
    struct, times, _, _, colors = read_node_parameters(args.in_file)
    times = sorted(list(set(times)))
    frames = range(len(times))
    uwt = read_network_log(args.uwt_path)
    wgt = read_network_log(args.wgt_path)

    # Map
    plt.figure(figsize=(7, 3.98))
    axes = equirectangular()
    write_title(args.title)
    map_gray_land(axes)
    figure_title_bar()
    lons = struct.loc[(times[0]), 'longitude']
    lats = struct.loc[(times[0]), 'latitude']
    pts = scatter(axes, lons, lats, colors)
    clk = write_clock(times[0] + ' UTC')

    # Update
    def update(idx):
        lons = struct.loc[(times[idx]), 'longitude']
        lats = struct.loc[(times[idx]), 'latitude']
        if axes.lines:
            del axes.lines[:]
        print_progress(idx, len(frames))
        pts.set_offsets(np.c_[lons, lats])
        clk.set_text(times[idx])
        if wgt:
            plot_wg(axes, wgt[idx], lons, lats)
        if uwt:
            plot_ug(axes, uwt[idx], lons, lats)
        return (pts, )

    # Animate
    print('Status: ')
    ani = anim.FuncAnimation(plt.gcf(), update, frames=frames, blit=True)
    if args.figure:
        plt.show()
    else:
        ani.save(args.out_dir + 'animation.mp4', dpi=200, writer=get_writer(24))
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
                        default='output/data.nc4',
                        nargs='?',
                        help='Path to input file')
    parser.add_argument('-o',
                        '--out_dir',
                        default='analysis/',
                        nargs='?',
                        help='Path to output directory')
    parser.add_argument('-d',
                        '--data_dir',
                        default='input/nc4/',
                        nargs='?',
                        help='Path to data directory')
    parser.add_argument('-t',
                        '--title',
                        default='',
                        nargs='?',
                        help='Title for the plot')
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
    check_dir(args.data_dir)
    if args.wgt_path:
        check_file(args.wgt_path)
    if args.uwt_path:
        check_file(args.uwt_path)
    return args


if __name__ == '__main__':
    main()
