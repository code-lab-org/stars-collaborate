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

"""Extracts target variables from NetCDF files."""

import argparse

from netCDF4 import Dataset
import numpy as np

from helper_system import check_dir
from helper_system import paths_with_extension


def main():
    """
    Extracts target variables from NetCDF files.

    Example:
        $ python3 util/extract.py -h
        usage: extract.py [-h] [-i [IN_DIR]] [-o [OUT_DIR]]

        optional arguments:
          -h, --help            show this help message and exit
          -i [IN_DIR], --in_dir [IN_DIR]
                                Path to input directory
          -o [OUT_DIR], --out_dir [OUT_DIR]
                                Path to output directory
    """

    # Input
    args = argparser()
    for idx, path in enumerate(paths_with_extension(args.in_dir, '.nc4')):
        data = Dataset(path)
        new = Dataset(f'{args.out_dir}{idx}.nc4', 'w', format='NETCDF4_CLASSIC')
        new.createDimension('lon', data.dimensions['lon'].size)
        new.createDimension('lat', data.dimensions['lat'].size)
        new.createDimension('time', None)
        for key in ['PRECTOT']:
            local = new.createVariable(key, np.float32, ('time', 'lat', 'lon'))
            local[:] = data.variables[key][:]


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
                        default='input/nc4/',
                        nargs='?',
                        help='Path to input directory')
    parser.add_argument('-o',
                        '--out_dir',
                        default='analysis/',
                        nargs='?',
                        help='Path to output directory')
    args = parser.parse_args()
    check_dir(args.out_dir)
    return args


if __name__ == '__main__':
    main()
