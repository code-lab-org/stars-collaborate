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

"""Helper methods for plotting."""

from datetime import datetime
import os

from cartopy.feature.nightshade import Nightshade
import cartopy.crs as ccrs
import cartopy.feature as cfeature
import matplotlib
from matplotlib.patches import Rectangle
import matplotlib.pyplot as plt


def arrow_tx(firstlon, lastlon, firstlat, lastlat, clip):
    """
    Draws an arrow to represent a communication channel.

    Args:
        firstlon (np.float64): Starting longitude (degrees)
        lastlon  (np.float64): Starting latitude (degrees)
        firstlat (np.float64): Ending longitude (degrees)
        lastlat  (np.float64): Ending latitude (degrees)
        clip     (bool): Whether visible outsidde the axes

    Examples:
        >>> arrow_tx(firstlon, lastlon, firstlat, lastlat, True)
    """
    plt.arrow(firstlon,
              firstlat,
              (lastlon-firstlon)*0.4,
              (lastlat-firstlat)*0.4,
              fc='g',
              ec='none',
              head_width=6,
              head_length=6,
              clip_on=clip,
              transform=ccrs.PlateCarree())
    plt.arrow(firstlon,
              firstlat,
              (lastlon-firstlon),
              (lastlat-firstlat),
              ec='g',
              lw=2,
              head_width=0,
              head_length=0,
              clip_on=clip,
              transform=ccrs.PlateCarree())
    if clip:
        plt.scatter(firstlon,
                    firstlat,
                    facecolor='w',
                    edgecolor='k',
                    s=15,
                    zorder=35,
                    marker='o')
        plt.scatter(lastlon,
                    lastlat,
                    facecolor='w',
                    edgecolor='k',
                    s=15,
                    zorder=35,
                    marker='o')


def arrows_tx(txlon, rxlon, txlat, rxlat):
    """
    Plots lines, or two if connection wraps around Earth.

    Args:
        txlon (np.float64): The beginning longitude (degrees)
        rxlon (np.float64): The ending longitude (degrees)
        txlat (np.float64): The beginning latitude (degrees)
        rxlat (np.float64): The ending latitude (degrees)

    Examples:
        >>> arrows_tx(axes, txlon, rxlon, txlat, rxlat)
    """
    if abs(txlon - rxlon) > 180:
        if txlon < 0:
            arrow_tx(txlon+360, rxlon, txlat, rxlat, True)
            arrow_tx(txlon, rxlon-360, txlat, rxlat, True)
        elif rxlon < 0:
            arrow_tx(txlon, rxlon+360, txlat, rxlat, True)
            arrow_tx(txlon-360, rxlon, txlat, rxlat, True)
    else:
        arrow_tx(txlon, rxlon, txlat, rxlat, True)


def centered(latitude, longitude):
    """
    Creates an azimuthal equidistant projection.

    Args:
        latitude (float): Latitude (degrees)
        longitude (float): Longitude (degrees)

    Returns:
        cartopy.mpl.geoaxes.GeoAxesSubplot

    Examples:
        >>> axes = centered(30, 20)
    """
    print('Preparing azimuthal equidistant projection')
    axes = plt.axes(projection=ccrs.NearsidePerspective(latitude,
                                                        longitude))
    axes.set_global()
    axes.coastlines(resolution='110m')
    axes.gridlines()
    return axes


def equirectangular():
    """
    Creates an equirectangular projection.

    Returns:
        cartopy.mpl.geoaxes.GeoAxesSubplot

    Examples:
        >>> axes = equirectangular()
    """
    print('Preparing equirectangular projection')
    axes = plt.axes(projection=ccrs.PlateCarree())
    axes.set_global()
    return axes


def figure_book_collaborate():
    """
    Creates a figure for creating a book of collaborative sequences.

    Examples:
        >>> figure_book_collaborate()
    """
    plt.subplots_adjust(left=0, right=1, top=1, bottom=0.088)
    llc = (-180, -107)
    dlon = 360
    dlat = 15
    plt.gca().add_patch(Rectangle(llc, dlon, dlat, clip_on=False, fill=None))
    plt.text(-175, -102, "Initial Measurement: ")
    plot_blue_line(-90, -83, -99.5, -99.5, False)

    plt.text(-71, -102, "Motion: ")
    plt.arrow(-40,
              -99.5,
              4,
              0,
              fc='k',
              ec='none',
              head_width=6,
              head_length=6,
              clip_on=False)
    plt.arrow(-40,
              -99.5,
              14,
              0,
              ec='k',
              lw=1.5,
              head_width=0,
              head_length=0,
              clip_on=False)

    plt.text(-20, -102, "Communication: ")
    arrow_tx(48, 62, -99.5, -99.5, False)

    plt.text(72, -102, "Adapted Measurement: ")
    plt.arrow(168,
              -99.5,
              7,
              0,
              ec="r",
              lw=3,
              head_width=0,
              head_length=0,
              clip_on=False)


def figure_plot_constellations():
    """
    Creates a figure for plotting constellations.

    Examples:
        >>> figure_plot_constellations()
    """
    plt.subplots_adjust(left=0, right=1, top=1, bottom=0.088)
    llc = (-180, -107)
    dlon = 360
    dlat = 15
    plt.gca().add_patch(Rectangle(llc, dlon, dlat, clip_on=False, fill=None))
    plt.text(-120, -102, "Train: ")
    plt.arrow(-90.5, -99.5, 11, 0, ec=(0, 0, 0), lw=5,
              head_width=0, head_length=0, clip_on=False)
    plt.arrow(-90, -99.5, 10, 0, ec=(1, 0.3, 0.3), lw=3,
              head_width=0, head_length=0, clip_on=False)

    plt.text(-40, -102, "Tandem: ")
    plt.arrow(-0.5, -99.5, 11, 0, ec=(0, 0, 0), lw=5,
              head_width=0, head_length=0, clip_on=False)
    plt.arrow(0, -99.5, 10, 0, ec=(0.3, 0.3, 1), lw=3,
              head_width=0, head_length=0, clip_on=False)

    plt.text(50, -102, "Walker-Delta: ")
    plt.arrow(109.5, -99.5, 11, 0, ec=(0, 0, 0), lw=5,
              head_width=0, head_length=0, clip_on=False)
    plt.arrow(110, -99.5, 10, 0, ec=(0.25, 0.85, 0.5), lw=3,
              head_width=0, head_length=0, clip_on=False)


def figure_plot_map():
    """
    Creates a figure for plotting a map.

    Examples:
        >>> figure_plot_map()
    """
    plt.subplots_adjust(left=0, right=1, top=1, bottom=0.088)
    llc = (-180, -107)
    dlon = 360
    dlat = 15
    plt.gca().add_patch(Rectangle(llc, dlon, dlat, clip_on=False, fill=None))
    plt.text(-150, -102, "A: ")  # "AAUSAT 4: "
    plt.arrow(-130.5, -99.5, 11, 0, ec=(0, 0, 0), lw=5,
              head_width=0, head_length=0, clip_on=False)
    plt.arrow(-130, -99.5, 10, 0, ec=(1, 0.5, 1), lw=3,
              head_width=0, head_length=0, clip_on=False)

    plt.text(-60, -102, "B: ")  # "UITMSAT-1: "
    plt.arrow(-35.5, -99.5, 11, 0, ec=(0, 0, 0), lw=5,
              head_width=0, head_length=0, clip_on=False)
    plt.arrow(-35, -99.5, 10, 0, ec=(1, 1, 0.6), lw=3,
              head_width=0, head_length=0, clip_on=False)

    plt.text(30, -102, "C: ")  # "HAVELSAT: "
    plt.arrow(49.5, -99.5, 11, 0, ec=(0, 0, 0), lw=5,
              head_width=0, head_length=0, clip_on=False)
    plt.arrow(50, -99.5, 10, 0, ec=(0.25, 0.85, 0.5), lw=3,
              head_width=0, head_length=0, clip_on=False)

    plt.text(120, -102, "D: ")  # "DUCHIFAT-1: "
    plt.arrow(139.5, -99.5, 11, 0, ec=(0, 0, 0), lw=5,
              head_width=0, head_length=0, clip_on=False)
    plt.arrow(140, -99.5, 10, 0, ec=(1, 0.3, 0.3), lw=3,
              head_width=0, head_length=0, clip_on=False)


def figure_title_bar():
    """
    Creates a figure with a title bar.

    Examples:
        >>> figure_title_bar()
    """
    plt.subplots_adjust(left=0, right=1, top=1, bottom=0)

    # # Bottom Bar
    llc = (-180, -102)
    dlon = 360
    dlat = 10
    plt.gca().add_patch(Rectangle(llc,
                                  dlon,
                                  dlat,
                                  clip_on=False,
                                  fill=None,
                                  zorder=1))
    # Top Bar
    llc = (-180, 92)
    dlon = 360
    dlat = 10
    plt.gca().add_patch(Rectangle(llc, dlon, dlat, clip_on=False, fill=None))
    plt.text(-172, 95.5, "Cube Satellite Positions", fontsize=7)


def get_writer(fps):
    """
    Obtains the default Writer for simulations.

    Args:
        fps (int): Frames per second

    Returns:
        matplotlib.animation.FFMpegWriter

    Examples:
        >>> w = get_writer(24)
    """
    wrt = matplotlib.animation.writers['ffmpeg']
    writer = wrt(fps=fps,
                 metadata=dict(artist='Me'),
                 bitrate=-1,
                 extra_args=['-pix_fmt', 'yuv420p'],
                 codec="libx264")
    return writer


def map_blue_marble(axes, month, resolution):
    """
    Draws the blue marble map.

    Args:
        axes       (cartopy.mpl.geoaxes.GeoAxesSubplot): The map
        month      (str): The month, 2 numeric digits
        resolution (str): Level of resolution ('high' or 'low')

    Examples:
        >>> map_blue_marble(axes, "09")
    """
    os.environ["CARTOPY_USER_BACKGROUNDS"] = "input/bm/"
    axes.background_img(name=month, resolution=resolution)


def map_gray_land(axes):
    """
    Adds neutral land features to a map.

    Args:
        axes (cartopy.mpl.geoaxes.GeoAxesSubplot): The map

    Examples:
        >>> axes = equirectangular()
        >>> map_gray_land(axes)
    """
    axes.add_feature(cfeature.NaturalEarthFeature('physical',
                                                  'land',
                                                  '110m',
                                                  edgecolor='#A1A1A1',
                                                  linewidth=0.25,
                                                  facecolor='#E6E6E6'))


def plot_blue_line(firstlon, lastlon, firstlat, lastlat, clip):
    """
    Draws a blue line.

    Args:
        firstlon (np.float64): Starting longitude (degrees)
        lastlon  (np.float64): Starting latitude (degrees)
        firstlat (np.float64): Ending longitude (degrees)
        lastlat  (np.float64): Ending latitude (degrees)
        clip     (bool): Whether visible outsidde the axes

    Examples:
        >>> plot_blue_line(lon1, lon2, lat1, lat2, False)
    """
    plt.arrow(firstlon,
              firstlat,
              (lastlon-firstlon),
              (lastlat-firstlat),
              ec="b",
              lw=3,
              head_width=0,
              head_length=0,
              clip_on=clip)


def plot_channel(axes, lonp, latp, color, zorder):
    """
    Draws a channel between two nodes.

    Args:
        axes   (cartopy.mpl.geoaxes.GeoAxesSubplot): The map
        lonp   (tuple): The longitude pair (degrees)
        latp   (tuple): The latitude pair (degrees)
        color  (tuple): The RGB triple
        zorder (np.float64): The z-order

    Examples:
        >>> plot_channel(axes, lonp, latp, color, zorder)
    """
    axes.plot(lonp, latp, c=color, lw=0.05, zorder=zorder)


def plot_data(axes, data):
    """
    Plots data on an equirectangular map.

    Args:
        axes (cartopy.mpl.geoaxes.GeoAxesSubplot): The map
        data (np.ndarray): The data

    Returns:
        matplotlib.image.AxesImage

    Examples:
        >>> axes = equirectangular()
        >>> files = paths_with_extension(args.in_dir, '.bin')
        >>> image = plot_data(axes, read_data(files[0]))
    """
    return axes.imshow(data, extent=(-180, 180, -90, 90))


def plot_directions(lons, lats):
    """
    Plots arrows at the middle of a track defined by lons and lats.

    Args:
        lons (list): Longitudes (degrees)
        lats (list): Latitudes (degrees)

    Examples:
        >>> plot_directions(lons, lats)
    """
    lastlon = lons[0]
    start = 0
    flag = False
    plt.scatter(lons[0],
                lats[0],
                facecolor='w',
                edgecolor='k',
                s=15,
                zorder=35,
                marker='o')
    plt.scatter(lons[-1],
                lats[-1],
                facecolor='w',
                edgecolor='k',
                s=15,
                zorder=35,
                marker='o')
    for idx, lon in enumerate(lons):
        if (abs(lon-lastlon) < 3) and (idx != len(lons)-1):
            if not flag:
                start = idx
                flag = True
        else:
            flag = False
            if (idx-start) > 50:
                mid = idx - int((idx-start) / 2)
                plt.arrow(lons[mid],
                          lats[mid],
                          lons[mid+1]-lons[mid],
                          lats[mid+1]-lats[mid],
                          fc="k",
                          ec="none",
                          head_width=6,
                          head_length=6)
        lastlon = lon


def plot_ug(axes, edges, lons, lats):
    """
    Draws colored lines between satellites based on channel weights.

    Args:
        axes  (cartopy.mpl.geoaxes.GeoAxesSubplot): The map
        edges (np.ndarray): The unweighted adjacency matrix
        lons  (list): Longitudes (degrees)
        lats  (list): Latitudes (degrees)

    Examples:
        >>> plot_ug(axes, uwts, range(num), range(num), lons, lats)
    """
    color = 'k'
    for txr, _ in enumerate(lons):
        for rxr, _ in enumerate(lons):
            if edges[txr][rxr]:
                txlon = lons[txr]
                rxlon = lons[rxr]
                latp = (lats[txr], lats[rxr])
                if abs(txlon - rxlon) > 180:
                    if txlon < 0:
                        lonp_1 = (txlon+360, rxlon)
                        lonp_2 = (txlon, rxlon-360)
                        plot_channel(axes, lonp_1, latp, color, 10)
                        plot_channel(axes, lonp_2, latp, color, 10)
                    elif rxlon < 0:
                        lonp_1 = (txlon, rxlon+360)
                        lonp_2 = (txlon-360, rxlon)
                        plot_channel(axes, lonp_1, latp, color, 10)
                        plot_channel(axes, lonp_2, latp, color, 10)
                else:
                    lonp = (txlon, rxlon)
                    plot_channel(axes, lonp, latp, color, 10)


def plot_wg(axes, weights, lons, lats):
    """
    Draws colored lines between satellites based on channel weights.

    Args:
        axes    (cartopy.mpl.geoaxes.GeoAxesSubplot): The map
        weights (np.ndarray): Weighted adjacency matrix
        lons    (list): Longitudes (degrees)
        lats    (list): Latitudes (degrees)

    Examples:
        >>> plot_wg(axes, ws, cs, ws, range(n), range(n), lns, lts)
    """
    zorder = 10
    colors = ['k']*(len(weights)**2)
    for txr, _ in enumerate(lons):
        for rxr, _ in enumerate(lons):
            if weights[txr][rxr]:
                if abs(lons[txr] - lons[rxr]) > 180:
                    if lons[txr] < 0:
                        lonp_1 = (lons[txr]+360, lons[rxr])
                        lonp_2 = (lons[txr], lons[rxr]-360)
                        plot_channel(axes,
                                     lonp_1,
                                     (lats[txr], lats[rxr]),
                                     colors[txr][rxr],
                                     zorder)
                        plot_channel(axes,
                                     lonp_2,
                                     (lats[txr], lats[rxr]),
                                     colors[txr][rxr],
                                     zorder)
                    elif lons[rxr] < 0:
                        lonp_1 = (lons[txr], lons[rxr]+360)
                        lonp_2 = (lons[txr]-360, lons[rxr])
                        plot_channel(axes,
                                     lonp_1,
                                     (lats[txr], lats[rxr]),
                                     colors[txr][rxr],
                                     zorder)
                        plot_channel(axes,
                                     lonp_2,
                                     (lats[txr], lats[rxr]),
                                     colors[txr][rxr],
                                     zorder)
                else:
                    lonp = (lons[txr], lons[rxr])
                    plot_channel(axes,
                                 lonp,
                                 (lats[txr], lats[rxr]),
                                 colors[txr][rxr],
                                 zorder)


def night(axes, clock):
    """
    Draws a night shade based on the time.

    Args:
        axes  (cartopy.mpl.geoaxes.GeoAxesSubplot): The map
        clock (str): The date-time string

    Returns:
        cartopy.mpl.feature_artist.FeatureArtist

    Examples:
        >>> axes = equirectangular()
        >>> night(axes, '2018-12-20 01:02:00')
    """
    date_time = datetime.strptime(clock, '%Y-%m-%d %H:%M:%S')
    return axes.add_feature(Nightshade(date_time), alpha=0.4)


def remove_margins():
    """
    Shrinks the margins more than plt.tight_layout() does.

    Examples:
        >>> remove_margins()
    """
    plt.subplots_adjust(left=0, right=1, top=1, bottom=0)


def scatter(axes, lons, lats, colors):
    """
    Creates a scatterplot of nodes over a map.

    Args:
        axes   (cartopy.mpl.geoaxes.GeoAxesSubplot): The map
        lons   (list): The longitudes
        lats   (list): The latitudes
        colors (list): The colors

    Returns:
        matplotlib.collections.PathCollection

    Examples:
        >>> scatter(axes, [0, 0.1, 0.2], [-3, -2.9, -2.8], colors)
    """
    return axes.scatter(lons,
                        lats,
                        lw=0.7,
                        facecolor=colors,
                        s=20,
                        edgecolor='k',
                        zorder=30,
                        marker='o')


def scatter_blue(axes, lons, lats):
    """
    Draws the adapted measurement on a map.

    Args:
        axes (cartopy.mpl.geoaxes.GeoAxesSubplot): The map
        lons (list): The longitudes (degrees)
        lats (list): The latitudes (degrees)

    Examples:
        >>> scatter_blue(axes, lons, lats)
    """
    axes.scatter(lons, lats, facecolor='r', s=7, zorder=30, marker='o')


def scatter_carrying(axes, lons, lats):
    """
    Draws positions of nodes carrying information.

    Args:
        axes (cartopy.mpl.geoaxes.GeoAxesSubplot): The map
        lons (list): The longitudes (degrees)
        lats (list): The latitudes (degrees)

    Examples:
        >>> scatter_carrying(axes, lons, lats)
    """
    axes.scatter(lons, lats, s=0.1, facecolor='k', zorder=20, marker='o')


def scatter_carrying_colored(axes, lons, lats, colors):
    """
    Draws positions of nodes carrying information.

    Args:
        axes   (cartopy.mpl.geoaxes.GeoAxesSubplot): The map
        lons   (list): The longitudes (degrees)
        lats   (list): The latitudes (degrees)
        colors (list): The colors

    Examples:
        >>> scatter_carrying_colored(axes, lons, lats)
    """
    axes.scatter(lons, lats, facecolor=colors, s=0.5, zorder=10, marker='o')


def scatter_red(axes, lons, lats):
    """
    Draws the initial measurement on a map.

    Args:
        axes (cartopy.mpl.geoaxes.GeoAxesSubplot): The map
        lons (list): The longitudes (degrees)
        lats (list): The latitudes (degrees)

    Examples:
        >>> scatter_red(axes, lons, lats)
    """
    axes.scatter(lons, lats, facecolor='b', s=7, zorder=30, marker='o')


def scatter_small_blue(axes, lats, lons):
    """
    Plots a track on a map.

    Args:
        axes (cartopy.mpl.geoaxes.GeoAxesSubplot): The map
        lats (list): The list of latitude values (degrees)
        lons (list): The list of longitude values (degrees)

    Examples:
        >>> scatter_small_blue(axes, [0, 0.1, 0.2], [-3, -2.9, -2.8])
    """
    axes.scatter(lons, lats, facecolor='b', s=1, zorder=10, marker='o')


def scatter_small_red(axes, lats, lons):
    """
    Plots a track on a map.

    Args:
        axes (cartopy.mpl.geoaxes.GeoAxesSubplot): The map
        lats (list): The list of latitude values (degrees)
        lons (list): The list of longitude values (degrees)

    Examples:
        >>> scatter_small_red(axes, [0, 0.1, 0.2], [-3, -2.9, -2.8])
    """
    axes.scatter(lons,
                 lats,
                 facecolor='r',
                 s=0.5,
                 zorder=10,
                 marker='o',
                 transform=ccrs.Geodetic())


def write_clock(clock):
    """
    Writes a clock on a plot.

    Args:
        clock (str): The clock string

    Returns:
        matplotlib.text.Text

    Examples:
        >>> write_clock('2018-09-09 13:00:00')
    """
    return plt.gcf().text(0.8, 0.965, clock + ' UTC', fontsize=7)


def write_names(geo):
    """
    Writes names on a plot.

    Args:
        geo (np.ndarray): The list of dictionaries

    Returns:
        list

    Examples:
        >>> geo = read_partial_node_log('log.bin')
        >>> write_names(geo[0])
    """
    names = []
    for index, name in enumerate(geo['name']):
        names.append(plt.annotate(str(name, 'utf-8'),
                                  (geo['lon'][index-1] + 2,
                                   geo['lat'][index-1]),
                                  fontsize=4,
                                  bbox=dict(boxstyle="round, pad=0.3", fc="w")))

    return names


def write_title(title):
    """
    Writes a title on a plot.

    Args:
        title (str): The title string

    Examples:
        >>> write_title('Cube Satellites')
    """
    plt.gcf().text(0.015, 0.965, title, fontsize=7)
