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

"""Plots a communication channel."""

import argparse

import cartopy.crs as ccrs
import matplotlib.gridspec as gridspec
import matplotlib.pyplot as plt

from helper_data import read_channel
from helper_plot import arrow_tx
from helper_system import check_dir
from helper_system import paths_which_contain


def main():
    """
    Creates a book containing plots of measurements.

    Notes:
        @image html raster/channel.png width=500

    Example:
        $ python3 util/plot_channel.py -h
        usage: plot_channel.py [-h] [-f] [-i [IN_DIR]] [-d [DATA_DIR]]
                               [-o [OUT_DIR]] [-n INDEX]

        optional arguments:
          -h, --help            show this help message and exit
          -f, --figure          Whether to show or save
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
    paths = sorted(paths_which_contain(args.in_dir, "channel"))
    path = paths[args.index]
    log, index, _ = read_channel(path)
    fig = plt.figure(figsize=(10, 8))
    outer = gridspec.GridSpec(2, 2, wspace=0.3, hspace=0.1)

    # General (top left)
    variables = ['los_speed', 'freq', 'distance', 'delay']
    labels = ['los_speed [m/s]', 'freq [Hz]',
              'distance [m]', 'delay [s]']
    inner = gridspec.GridSpecFromSubplotSpec(len(variables),
                                             1,
                                             subplot_spec=outer[0],
                                             wspace=1,
                                             hspace=0.7)

    for index, _ in enumerate(variables):
        axes = plt.Subplot(fig, inner[index])
        plot_series(axes, log[variables[index]], [], labels[index])
        fig.add_subplot(axes)
        index += 1

    # Map (top right)
    geo = (log['tx_lon'][0], log['rx_lon'][0],
           log['tx_lat'][0], log['rx_lat'][0])
    axes = plt.subplot(222, projection=ccrs.PlateCarree(log['tx_lon'][0]))
    axes.stock_img()
    axes.scatter(geo[0], geo[2], zorder=20, transform=ccrs.PlateCarree())
    axes.scatter(geo[1], geo[3], zorder=20, transform=ccrs.PlateCarree())
    arrow_tx(*geo, False)
    fig.add_subplot(axes)

    # Transmitter (bottom left)
    variables = ['tx_buffer', 'tx_alt', 'tx_gain', 'tx_power']
    lbls = ['tx_buffer [B]', 'tx_alt [m]', 'tx_gain [dB]', 'tx_power [W]']
    inner = gridspec.GridSpecFromSubplotSpec(len(variables),
                                             1,
                                             subplot_spec=outer[2],
                                             wspace=1,
                                             hspace=0.7)
    for index, _ in enumerate(variables):
        axes = plt.Subplot(fig, inner[index])
        plot_series(axes, log[variables[index]], [], lbls[index])
        fig.add_subplot(axes)
        index += 1

    # Receiver (bottom right)
    variables = ['rx_buffer', 'rx_alt', 'rx_gain', 'rx_power']
    lbls = ['rx_buffer [B]', 'rx_alt [m]', 'rx_gain [dB]', 'rx_power [W]']
    inner = gridspec.GridSpecFromSubplotSpec(len(variables),
                                             1,
                                             subplot_spec=outer[3],
                                             wspace=1,
                                             hspace=0.7)
    for index, _ in enumerate(variables):
        axes = plt.Subplot(fig, inner[index])
        plot_series(axes, log[variables[index]], [], lbls[index])
        fig.add_subplot(axes)
        index += 1

    plt.subplots_adjust(0.11, 0.01, 0.99, 0.95)
    if args.figure:
        plt.show()
    else:
        plt.savefig('analysis/channel.pdf')
        plt.savefig('analysis/channel.png', dpi=200)
    plt.close()


def plot_series(axes, data, ticks, ylabel):
    """
    Plots charging data.

    Args:
        axes   (matplotlib.axes._subplots.AxesSubplot): Subplot
        data   (list): Time series data to plot
        ticks  (list): Ticks for x axis
        ylabel (str): Label for y axis
    """
    axes.set_xticks(ticks)
    axes.set_xticklabels([])
    axes.set_yticks([data.min(), data.max()])
    axes.plot(data)
    axes.set_ylabel(ylabel, rotation=0)
    axes.yaxis.set_label_coords(0.5, 1.1)
    #  axes.yaxis.set_label_position("right")
    axes.grid()


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
