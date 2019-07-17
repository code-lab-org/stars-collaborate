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

"""Helper methods for system operations."""

import glob
import os
import sys

from PyPDF2 import PdfFileMerger


def check_file(path):
    """
    Checks if a file exists.

    Args:
        path (str): A file path

    Examples:
        >>> check_file('output/log.bin')
    """
    if not os.path.isfile(path):
        sys.exit('Error: "{}" is not a file'.format(path))


def check_not_file(path):
    """
    Checks if a file does not exist.

    Args:
        path (str): A file path

    Examples:
        >>> check_file('output/log.bin')
    """
    if os.path.isfile(path):
        sys.exit('Error: "{}" is already a file'.format(path))


def check_dir(path):
    """
    Checks if a directory exists.

    Args:
        path (str): A directory path

    Examples:
        >>> check_dir('output/')
    """
    if not os.path.isdir(path):
        sys.exit('Error: "{}" is not a directory'.format(path))
    if path[-1] != '/':
        sys.exit('Error: "{}" does not end with a "/"'.format(path))


def check_not_dir(path):
    """
    Checks if a directory does not exist.

    Args:
        path (str): A directory path

    Examples:
        >>> check_not_dir('output/')
    """
    if os.path.isdir(path):
        sys.exit('Error: "{}" is an existing directory'.format(path))


def check_month(month):
    """
    Checks if a month is valid.

    Args:
        month (str): A month (2 digits)

    Examples:
        >>> check_month('08')
    """
    if int(month) not in [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]:
        sys.exit('Error: "{}" is not in the range 1-12'.format(month))


def merge_pdfs(paths, out_dir, filename):
    """
    Concatenates PDF files to produce a single file.

    Args:
        paths    (list): The input files
        out_dir  (str): The output directory
        filename (str): The output filename

    Examples:
        >>> merge_pdfs(['0.pdf, 1.pdf'], analysis/, 'book.pdf')
    """
    merger = PdfFileMerger()
    for pdf in paths:
        merger.append(pdf)
    merger.write(out_dir + filename)


def only_extension(path):
    """
    Obtains the file extension from a path.

    Args:
        path (str): Path to a file

    Returns:
        string

    Example:
        >>> ext = only_name("path/to/a/file.txt")
        >>> ext
        .txt
    """
    return os.path.splitext(os.path.basename(path))[1]


def only_name(path):
    """
    Obtains the filename from a path.

    Args:
        path (str): Path to a file

    Returns:
        string

    Example:
        >>> name = only_name("path/to/a/file.txt")
        >>> name
        file.txt
    """
    return os.path.splitext(os.path.basename(path))[0]


def paths_which_contain(directory, substring):
    """
    Obtains a sorted list of files in a directory with an extension.

    Args:
        directory (str): A directory path
        substring (str): Substring to find

    Returns:
        list

    Examples:
        >>> files = paths_which_contain('output/', 'substring')
        >>> files
        ['1234substring7890.mp4']
    """
    all_files = [f for f in glob.iglob('{}/*'.format(directory))]
    files = []
    for a_file in all_files:
        if substring in a_file:
            files.append(a_file)
    if not files:
        sys.exit('Error: "{}" has no "{}"'.format(directory, substring))
    print('Loading {} files from "{}"'.format(len(files), directory))
    return sorted(files)


def paths_with_extension(directory, extension):
    """
    Obtains a sorted list of files in a directory with an extension.

    Args:
        directory (str): A directory path
        extension (str): A file extension

    Returns:
        list

    Examples:
        >>> files = paths_with_extension('Videos/', '.mp4')
        >>> files
        ['animation.mp4', 'visualization.mp4']
    """
    files = [f for f in glob.iglob('{}/*{}'.format(directory, extension))]
    if not files:
        sys.exit('Error: "{}" has no "*{}"'.format(directory, extension))
    print('Loading {} files from "{}"'.format(len(files), directory))
    return sorted(files)


def paths_with_prefix(directory, prefix):
    """
    Obtains a sorted list of files in a directory with a prefix.

    Args:
        directory (str): A directory path
        prefix    (str): A file prefix

    Returns:
        list

    Examples:
        >>> files = paths_with_prefix('data/', 'log')
        >>> files
        ['log_1.bin', 'log_2.txt']
    """
    files = [f for f in glob.iglob('{}/{}*'.format(directory, prefix))]
    if not files:
        sys.exit('Error: "{}" has no "{}*"'.format(directory, prefix))
    print('Loading {} files from "{}"'.format(len(files), directory))
    return sorted(files)


def print_progress(frame, num_frames):
    """
    Prints the percentage complete to standard output.

    Args:
        frame      (int): The current frame
        num_frames (int): The total number of frames

    Examples:
        >>> print_progress(1, 100)
    """
    progress = int(frame / num_frames * 100)
    print('\r{}%'.format(progress), end='')


def write_gitignore(directory):
    """
    Writes a .gitignore file to the specified directory.

    Args:
        directory (str): A directory path

    Examples:
        >>> write_gitignore('new_empty_directory/')
    """
    with open(directory + '.gitignore', 'w') as output_file:
        output_file.write('# Ignore everything in this directory\n'
                          '*\n'
                          '# Except this file\n'
                          '!.gitignore\n')
