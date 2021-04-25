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

"""Combines NetCDF files."""

import argparse

from netCDF4 import Dataset
import numpy as np

from helper_system import check_dir
from helper_system import paths_with_prefix


def main():
    """
    Combines NetCDF files.

    Example:
        $ python3 util/combine.py -h
        usage: combine.py [-h] [-i [IN_DIR]] [-o [OUT_DIR]]


        optional arguments:
          -h, --help            show this help message and exit
          -i [IN_DIR], --in_dir [IN_DIR]
                                Path to input directory
          -o [OUT_DIR], --out_dir [OUT_DIR]
                                Path to output directory
    """

    # Input
    args = argparser()
    cloud_files = paths_with_prefix(args.in_dir, 'TAUTOT')
    rain_files = paths_with_prefix(args.in_dir, 'PRECTOT')
    for idx, cloud_file in enumerate(cloud_files):
        cloud = Dataset(cloud_file)
        rain = Dataset(rain_files[idx])
        new = Dataset(f'{args.out_dir}{idx}.nc4', 'w', format='NETCDF4_CLASSIC')
        new.createDimension('lon', cloud.dimensions['lon'].size)
        new.createDimension('lat', cloud.dimensions['lat'].size)
        new.createDimension('time', None)
        dimensions = ('time', 'lat', 'lon')
        new_cloud = new.createVariable('TAUTOT', np.float32, dimensions)
        new_cloud[:] = cloud.variables['TAUTOT'][:]
        new_rain = new.createVariable('PRECTOT', np.float32, dimensions)
        new_rain[:] = rain.variables['PRECTOT'][:]

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
                        default='input/seed/',
                        nargs='?',
                        help='Path to input directory')
    parser.add_argument('-o',
                        '--out_dir',
                        default='input/nc4/',
                        nargs='?',
                        help='Path to output directory')
    args = parser.parse_args()
    check_dir(args.out_dir)
    return args


if __name__ == '__main__':
    main()
