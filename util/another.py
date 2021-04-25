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
import pathlib as pl
import pickle
import sys

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
    s_rain_files = paths_which_contain('output/s/', 'PRECTOT')
    s_rain_files = [f for f in s_rain_files if f[-4:] == '.nc4']
    s_optical_files = paths_which_contain('output/s/', 'TAUTOT')
    s_optical_files = [f for f in s_optical_files if f[-4:] == '.nc4']
    
    if len(s_rain_files) == 0:
        sys.exit('Error: no simple rain measurements.')
    if len(s_optical_files) == 0:
        sys.exit('Error: no simple optical measurements.')
        
    for substr, num_sources in [('c1', 50), ('c2', 1)]:
        c_rain_files = paths_which_contain(f'output/{substr}/', 'PRECTOT')
        c_optical_files = paths_which_contain(f'output/{substr}/', 'TAUTOT')
        c_optical_files = [f for f in c_optical_files if f[-4:] == '.nc4']
        c_optical_files = [f for f in c_optical_files if int(f[23:28]) >= num_sources]
        c_rain_files = [f for f in c_rain_files if f[-4:] == '.nc4']

        if len(c_rain_files) == 0:
            sys.exit('Error: no collaborative rain measurements.')
        if len(c_optical_files) == 0:
            sys.exit('Error: no collaborative optical measurements.')

        def load_data(data, files):
            for filename in files:
                log, index, _ = read_measurement(filename)
                data.append(np.array(log['measurement']))

        s_rain_data = []
        s_optical_data = []
        c_optical_data = []
        c_rain_data = []
        load_data(s_rain_data, s_rain_files)
        load_data(c_rain_data, c_rain_files)
        load_data(s_optical_data, s_optical_files)
        load_data(c_optical_data, c_optical_files)

        def test_rain(data):
            counts = []
            for element in data:
                value = 0
                if (element > 0.0006).any():
                    value = 1
                counts.append(value)
            return np.sum(counts)/len(data)*100.0

        s_rain_percent = test_rain(s_rain_data)
        print(f'Simple Rain: {s_rain_percent:.2f}%')
        c_rain_percent = test_rain(c_rain_data)
        print(f'Collaborative Rain: {c_rain_percent:.2f}%')

        def test_optical(data):
            counts = []
            for element in data:
                value = 0
                if (element == 0.0).all():
                    value = 1
                counts.append(value)
            return np.sum(counts)/len(data)*100

        s_optical_percent = test_optical(s_optical_data)
        print(f'Simple Optical: {s_optical_percent:.2f}%')
        c_optical_percent = test_optical(c_optical_data)
        print(f'Collaborative Optical: {c_optical_percent:.2f}%')

        fig, axes = plt.subplots(figsize=(8,2.5))
        plt.bar(1, s_rain_percent, color='b', width=0.1)
        plt.bar(2, c_rain_percent, color='r', width=0.1)
        plt.bar(3, s_optical_percent, color='b', width=0.1)
        plt.bar(4, c_optical_percent, color='r', width=0.1)
        plt.xlim([0.5, 4.5])
        plt.xticks([1, 2, 3, 4])
        plt.ylim([0, 100])
        axes.set_xticklabels(['Isolated\nPrecipitation\n(Medium Rainfall)',
                              'Collaborative\nPrecipitation\n(Medium Rainfall)',
                              'Isolated\nOptical\n(Unobstructed)',
                              'Collaborative\nOptical\n(Unobstructed)'])
        plt.ylabel('Measurements')
        plt.title('Science-Value Improvement')
        plt.grid()
        plt.savefig(f'analysis/{substr}_bar.png', dpi=300, bbox_inches='tight',
                    pad_inches=0.05)
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
