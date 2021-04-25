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

"""Data-processing helper methods."""

import os
import random

from matplotlib.colors import Normalize
import matplotlib
import matplotlib.pyplot as plt
from netCDF4 import Dataset
import numpy as np
import pandas as pd


def archive_channel(path):
    """
    Reads a channel log into a pandas structure.

    Args:
        path (str): A file path

    Returns:
        DataFrame

    Examples:
        >>> dataframe = archive_channel('log.nc4')
    """
    data = Dataset(path)
    stamps = []
    structure = []
    dtv = ['year', 'month', 'day', 'hour', 'minute', 'second', 'microsecond']
    tup = list(zip(*[list(data.variables[v][:].data) for v in dtv]))
    times = range(len(data.dimensions['ticks']))
    stamps = ['{}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}:{:02d}'.format(*tup[idx])
              for idx in times]
    structure = pd.DataFrame({'tick': data.variables['time'][:],
                              'los_speed': data.variables['los_speed'][:],
                              'omega': data.variables['omega'][:],
                              'freq': data.variables['omega'][:]/(2*np.pi),
                              'distance': data.variables['distance'][:],
                              'delay': data.variables['delay'][:],
                              'data_rate': data.variables['data_rate'][:],
                              'tx_idx': data.variables['tx_idx'][:],
                              'tx_buffer': data.variables['tx_buffer'][:],
                              'tx_lon': data.variables['tx_lon'][:],
                              'tx_lat': data.variables['tx_lat'][:],
                              'tx_alt': data.variables['tx_alt'][:],
                              'tx_gain': data.variables['tx_gain'][:],
                              'tx_power': data.variables['tx_power'][:],
                              'rx_idx': data.variables['rx_idx'][:],
                              'rx_buffer': data.variables['rx_buffer'][:],
                              'rx_lon': data.variables['rx_lon'][:],
                              'rx_lat': data.variables['rx_lat'][:],
                              'rx_alt': data.variables['rx_alt'][:],
                              'rx_gain': data.variables['rx_gain'][:],
                              'rx_power': data.variables['rx_power'][:]},
                             index=stamps)
    structure['tx_lat'] = np.degrees(structure['tx_lat'])
    structure['tx_lon'] = np.degrees(structure['tx_lon'])
    structure['rx_lat'] = np.degrees(structure['rx_lat'])
    structure['rx_lon'] = np.degrees(structure['rx_lon'])
    structure.index.names = ['time']
    return structure


def archive_measurement(path):
    """
    Reads a measurement log into a pandas structure.

    Args:
        path (str): A file path

    Returns:
        DataFrame

    Examples:
        >>> dataframe = archive_measurement('log.nc4')
    """
    data = Dataset(path)
    stamps = []
    structure = []
    dtv = ['year', 'month', 'day', 'hour', 'minute', 'second', 'microsecond']
    tup = list(zip(*[list(data.variables[v][:].data) for v in dtv]))
    times = range(len(data.dimensions['ticks']))
    stamps = ['{}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}:{:02d}'.format(*tup[idx])
              for idx in times]
    structure = pd.DataFrame({'tick': data.variables['time'][:],
                              'latitude': data.variables['latitude'][:],
                              'longitude': data.variables['longitude'][:],
                              'altitude': data.variables['altitude'][:],
                              'measurement': data.variables['measurement'][:],
                              'resolution': data.variables['resolution'][:],
                              'index': data.variables['index'][:]},
                             index=stamps)
    structure['latitude'] = np.degrees(structure['latitude'])
    structure['longitude'] = np.degrees(structure['longitude'])
    structure.index.names = ['time']
    return structure


def archive_node_parameters(path):
    """
    Reads a node parameter log into a pandas structure.

    Args:
        path (str): A file path

    Returns:
        DataFrame

    Examples:
        >>> dataframe = archive_node_parameters('log.nc4')
    """
    data = Dataset(path)
    stamps = []
    tup = list(zip(*[list(data.variables[v][:].data) for v in data.variables]))
    times = range(len(data.dimensions['time']))
    stamps = ['{}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}:{:02d}'.format(*tup[idx])
              for idx in times]
    frames = dict()
    for group in data.groups:
        current_group = data.groups[group]
        frm = pd.DataFrame({'area': current_group['area'][:],
                            'charging': current_group['charging'][:],
                            'constellation': current_group['constellation'][:],
                            'energy': current_group['energy'][:],
                            'index': current_group['index'][:],
                            'latitude': current_group['latitude'][:],
                            'longitude': current_group['longitude'][:],
                            'mode': current_group['mode'][:],
                            'num_neighbors': current_group['num_neighbors'][:]},
                           index=stamps)
        frm['latitude'] = np.degrees(frm['latitude'])
        frm['longitude'] = np.degrees(frm['longitude'])
        frames['{}'.format(group)] = frm
    structure = pd.concat(frames.values(), axis=1, keys=frames.keys()).stack(0)
    structure.index.names = ['time', 'node']
    return structure


def generate_colors(data, cmap, alphas):
    """
    Creates an image from data, the colormap, and transparency data.

    Args:
        data   (numpy.ndarray): Array of data
        cmap   (matplotlib.colors.LinearSegmentedColormap): Colormap
        alphas (numpy.ndarray): The transparency data

    Returns:
        numpy.ndarray

    Examples:
        >>> image = generate_colors
        >>> data = read_32bit_float(path)
        >>> data = data.reshape(-1, 5760)
        >>> alphas = data/np.max(data)
        >>> alphas = alphas**0.3
        >>> cmap = plt.cm.gray
        >>> colors = generate_colors(data, cmap, alphas)
        >>> colors
        array([[[1. , 1.  , 1.   , 0.17410637],
                ...,
                [1. , 1.  , 1.   , 0.37406474]],...])
    """
    colors = Normalize(0, 1, clip=True)(data)
    colors = cmap(colors)
    colors[..., -1] = alphas
    return colors


def generate_colors_charging(log):
    """
    Creates list of colors based on charging status.

    Args:
        log (numpy.ndarray): The charge values

    Returns:
        list

    Examples:
        >>> pts = scatter(axes, lons, lats, colors)
        >>> pts.set_facecolor(generate_colors_charging(log[idx]))
    """
    colors = []
    for element in log:
        if element <= 0:
            colors.append((1, 0, 0))
        else:
            colors.append((0, 1, 0))
    return colors


def generate_colors_energy(log):
    """
    Creates list of colors based on energy stored in battery.

    Args:
        log (numpy.ndarray): The energy values

    Returns:
        list

    Examples:
        >>> pts = scatter(axes, lons, lats, colors)
        >>> pts.set_facecolor(generate_colors_energy(log[idx]))
    """
    minimum = np.min(log)
    maximum = np.max(log)
    colors = []
    for element in log:
        if element < 0:
            colors.append((((element)/(minimum))**10, 0, 0))
        elif element > 0:
            colors.append((0, ((element)/(maximum))**10, 0))
        else:
            colors.append((0, 0, 0))
    return colors


def generate_colors_mode(log):
    """
    Creates list of colors based on mode.

    Args:
        log (numpy.ndarray): The mode values

    Returns:
        list

    Examples:
        >>> pts = scatter(axes, lons, lats, colors)
        >>> pts.set_facecolor(generate_colors_mode(log[idx]))
    """
    colors = []
    for point in log:
        if point['mode'] == 1:
            colors.append((1, 0, 0))
        elif point['mode'] == 2:
            colors.append((0, 0, 1))
        else:
            colors.append((0, 0, 0))
    return colors


def generate_random_colors(num_colors):
    """
    Obtains a list of random RGB colors 'num_colors' long.

    Args:
        num_colors (int): The number of colors

    Returns:
        list

    Examples:
        >>> generate_random_colors(3)
        [(0.5940777158529069, 0.8648533783505895, 0.7589723913393599),
         (0.3002579911012273, 0.8657590658827679, 0.3411542723718176),
         (0.04461779956705281, 0.9596009561248078, 0.528176058498729)]
    """
    return [(random.uniform(0, 1),
             random.uniform(0, 1),
             random.uniform(0, 1)) for k in range(num_colors)]


def generate_weighted_colors(weights, maximum, minimum):
    """
    Obtains a list of RGB colors proportional to weights.

    Args:
        weights (numpy.ndarray): The array of weights
        maximum (numpy.float64): The maximum weight
        minimum (numpy.float64): The minimum weight

    Returns:
        numpy.ndarray

    Examples:
        >>> np.shape(weights, 10, 0)
        (2, 3)
        >>> colors = get_generate_random_colors(weights)
        >>> colors
        array([[[0.00141655, 0.        , 0.        ],
                [0.37368507, 0.        , 0.        ],
                [0.        , 0.        , 0.21979115]],

               [[1.        , 0.        , 0.        ],
                [0.        , 0.        , 1.        ],
                [0.        , 0.        , 0.04475385]]])
    """
    rows = np.shape(weights)[0]
    columns = np.shape(weights)[1]
    colors = np.zeros((rows, columns), dtype=(float, 3))
    for row in range(0, rows):
        for column in range(0, columns):
            value = weights[row][column]
            if value < 0:
                colors[row][column] = (((value)/(minimum)), 0, 0)
            elif value > 0:
                colors[row][column] = (0,
                                       (1-((value)/(maximum-minimum)))**1.3,
                                       0)
            else:
                colors[row][column] = (0, 0, 0)
    return colors


def read_antenna(path):
    """
    Reads a binary file containing antenna gain data.

    Args:
        path (str): A file path

    Returns:
        list
        numpy.ndarray
        numpy.ndarray
        range

    Examples:
        >>> gain, t_mat, p_mat, o_vec = read_antenna('output/patch.bin')
    """
    data = Dataset(path)
    num_p = data.dimensions['phi'].size
    num_t = data.dimensions['theta'].size
    t_mat, p_mat = np.meshgrid(np.linspace(0, 2*np.pi, num_t),
                               np.linspace(0, np.pi, num_p))
    gain = data.variables['gain'][:]
    return gain, t_mat, p_mat


def read_ascii_file(path):
    """
    Reads a text file into a list with one line per element.

    Args:
        path (str): A file path

    Returns:
        list

    Examples:
        >>> lines = read_ascii_file(path)
        >>> lines
        ['Line 1', 'Line 2', 'Line 3']
    """
    print('Processing "{}"'.format(path))
    lines = []
    try:
        with open(path) as input_file:
            whole_lines = input_file.readlines()
            lines = [whole_line.strip('\n ') for whole_line in whole_lines]
    except FileNotFoundError:
        print('Error: {} does not exist'.format(path))
    return lines


def read_channel(path):
    """
    Reads a channel DataFrame from a '.pkl' or a '.nc4' file.

    Args:
        path (str): A file path

    Returns:
        DataFrame
        List
        List

    Example:
        >>> structure, index, columns = read_channel("channel.nc4")
    """
    prefix = os.path.splitext(path)[0]
    archive = prefix + ".pkl"
    if os.path.exists(archive):
        print('Processing archive "{}"'.format(archive))
        structure = pd.read_pickle(archive)
    else:
        print('Processing log "{}"'.format(path))
        structure = archive_channel(path)
        structure.to_pickle(archive)
    times = structure.index.get_level_values('time')
    return structure, times, structure.columns


def read_cloud_data(path):
    """
    Reads cloud data and processes it for viewing on a map.

    Args:
        path (str): A file path

    Returns:
        np.ndarray

    Examples:
        >>> image = read_cloud_data('data.bin')
        >>> image
        array([[[1., 1., 1., 0.1741736], ...]])
    """
    data = Dataset(path)
    temp = data.variables['TAUTOT'][:]
    temp[temp < 4] = 0
    data = np.reshape(temp, (-1, 5760))
    alphas = data/np.max(data)
    alphas = alphas**0.3
    cmap = plt.cm.gray
    colors = generate_colors(data, cmap, alphas)
    return colors


def read_data(path, variable):
    """
    Reads data and processes based on its origin (directory).

    Args:
        path     (str): A file path
        variable (str): A variable name

    Returns:
        np.ndarray

    Examples:
        >>> image = read_data('TAUTOT_0.bin')
        >>> image
        array([[[1., 1., 1., 0.1741736], ...]])
    """
    colors = np.array([])
    if variable == 'TAUTOT':
        colors = read_cloud_data(path)
    elif variable == 'PRECTOT':
        colors = read_precipitation_data(path)
    return colors


def read_measurement(path):
    """
    Reads a measurement DataFrame from a '.pkl' or a '.nc4' file.

    Args:
        path (str): A file path

    Returns:
        DataFrame
        List
        List

    Example:
        >>> structure, index, columns = read_measurement("measurement.nc4")
    """
    prefix = os.path.splitext(path)[0]
    archive = prefix + ".pkl"
    if os.path.exists(archive):
        print('Processing archive "{}"'.format(archive))
        structure = pd.read_pickle(archive)
    else:
        print('Processing log "{}"'.format(path))
        structure = archive_measurement(path)
        structure.to_pickle(archive)
    times = structure.index.get_level_values('time')
    return structure, times, structure.columns


def read_node_parameters(path):
    """
    Reads a node parameter DataFrame from a '.pkl' or a '.nc4' file.

    Args:
        path (str): A '.nc4' file path

    Returns:
        DataFrame
        List
        List
        List
        List

    Examples:
        >>> log, time, nodes, cols = read_node_parameters('log.nc4')
        >>> log
                                       area  charging    energy latitude
        time                 node
        2019-4-23 12:41:19:0 0     0.021790         0 72.237420  1.39452
                             1     0.000000         0 72.237420 -0.64753
                             2     0.000000         0 72.237420  0.85850
        2019-4-23 12:41:20:0 0     0.021790         1 72.237420  1.39398
                             1     0.000000         0 72.235686 -0.64855
                             2     0.000000         0 72.235686  0.85954
        2019-4-23 12:41:21:0 0     0.021789         1 72.237420  1.39345
                             1     0.000000         0 72.233952 -0.64956
                             2     0.000000         0 72.233952  0.86058
        2019-4-23 12:41:22:0 0     0.021789         1 72.237420  1.39291
                             1     0.000000         0 72.232218 -0.65058
                             2     0.000000         0 72.232218  0.86162
        >>> time
        Index(['2019-4-23 12:41:19:0', '2019-4-23 12:41:20:0', ...],
              dtype='object', name='time')
        >>> nodes
        Index(['0', '1', '2', '0', '1', '2', ...],
              dtype='object', name='node')
        >>> columns
        Index(['area', 'charging', 'constellation', ...],
              dtype='object')
    """
    prefix = os.path.splitext(path)[0]
    archive = prefix + ".pkl"
    if os.path.exists(archive):
        print('Processing archive "{}"'.format(archive))
        structure = pd.read_pickle(archive)
    else:
        print('Processing log "{}"'.format(path))
        structure = archive_node_parameters(path)
        structure.to_pickle(archive)
    times = structure.index.get_level_values('time')
    nodes = structure.index.get_level_values('node')
    columns = structure.columns
    groups = sorted(structure.loc[(times[0], nodes), 'constellation'].values)
    unique_groups = sorted(set(groups))
    unique_colors = generate_random_colors(len(unique_groups))
    colors = []
    for index in groups:
        colors.append(unique_colors[index])
    return structure, times, nodes, columns, colors


def read_precipitation_data(path):
    """
    Reads cloud data and processes it with NWS colors.

    Args:
        path (str): A file path

    Returns:
        np.ndarray

    Examples:
        >>> image = read_precipitation_data('data.bin')
        >>> image
        array([[[1., 1., 1., 0.1741736], ...]])
    """
    nws = (["#04e9e7", "#019ff4", "#0300f4", "#01c501", "#fdf802", "#fd9500"] +
           ["#d40000", "#f800fd", "#9854c6", "#9854c6", "#fdfdfd", "#fdfdfd"])
    precip_colormap = matplotlib.colors.ListedColormap(nws)
    data = Dataset(path)
    temp = data.variables['PRECTOT'][:]
    data = np.reshape(temp, (-1, 5760))
    data = data/np.max(data)
    data = data**0.16
    alphas = data
    alphas[alphas < 0.25] = 0
    alphas = alphas**0.7
    cmap = precip_colormap
    colors = generate_colors(data, cmap, alphas)
    return colors


def read_network_log(path):
    """
    Reads an unweighted adjacency matrix from a netcdf file.

    Args:
        path (str): The absolute path of the file

    Returns:
        list

    Examples:
        >>> edges = read_network_log('network.nc4')
        >>> edges
        [array([[...]]), ... ]
    """
    graphs = []
    if path:
        print('Processing "{}"'.format(path))
        data = Dataset(path)
        for idx in range(len(data.dimensions['time'])):
            graph = np.array(data.variables['edges'][:, :, idx])
            graph = graph**0.5
            graphs.append(graph)
    return graphs
