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

"""Plots the gain pattern of an antenna."""

import argparse

from matplotlib import colors
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

from helper_data import read_antenna
from helper_plot import remove_margins
from helper_system import check_dir
from helper_system import check_file


def main():
    """
    Plots the gain pattern of an antenna.

    Notes:
        @image html raster/antennas.png width=500

    Example:
        $ python3 util/plot_antenna.py -h
        usage: plot_antenna.py [-h] [-f] [-i [IN_FILE]] [-o [OUT_DIR]]

        optional arguments:
          -h, --help            show this help message and exit
          -f, --figure          Whether to show or save
          -i [IN_FILE], --in_file [IN_FILE]
                                Path to input file
          -o [OUT_DIR], --out_dir [OUT_DIR]
                                Path to output directory
    """

    # Inputs
    args = argparser()
    gain, t_mat, p_mat = read_antenna(args.in_file)

    # Plots
    maximum = np.amax(gain)
    minimum = np.amin(gain)
    norm = colors.Normalize(minimum, maximum)
    colormap = plt.cm.ScalarMappable(norm=norm, cmap='jet')
    colormap.set_array([])
    fig = plt.figure(figsize=(9, 9))
    axes = Axes3D(fig)
    axes.set_xticklabels([])
    axes.set_yticklabels([])
    axes.set_zticklabels([])
    axes.set_xlim3d(-1, 1)
    axes.set_ylim3d(-1, 1)
    axes.set_zlim3d(-1, 1)
    #  axes.set_xlim(-maximum/2, maximum/2)
    #  axes.set_ylim(-maximum/2, maximum/2)
    #  axes.set_zlim(0, maximum)
    axes.set_xlim(-maximum, maximum)
    axes.set_ylim(-maximum, maximum)
    axes.set_zlim(-maximum, maximum)
    axes.plot_surface(gain*np.sin(p_mat)*np.cos(t_mat),
                      gain*np.sin(p_mat)*np.sin(t_mat),
                      gain*np.cos(p_mat),
                      rstride=1,
                      cstride=1,
                      linewidth=0.1,
                      antialiased=True,
                      alpha=1,
                      facecolors=colormap.to_rgba(gain),
                      vmin=minimum,
                      vmax=maximum,
                      shade=True)
    axes.view_init(20, 20)
    remove_margins()
    plt.subplots_adjust(left=0.4, right=0.6, top=0.6, bottom=0.4)
    if args.figure:
        plt.show()
    else:
        plt.savefig('{}antenna.pdf'.format(args.out_dir))


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
                        default='output/antenna.nc4',
                        nargs='?',
                        help='Path to input file')
    parser.add_argument('-o',
                        '--out_dir',
                        default='analysis/',
                        nargs='?',
                        help='Path to output directory')
    args = parser.parse_args()
    check_file(args.in_file)
    check_dir(args.out_dir)
    return args


if __name__ == '__main__':
    main()
