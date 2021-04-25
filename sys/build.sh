#!/bin/bash
#
# Copyright (C) 2019 The Ohio State University
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

VERBOSE=false
CPPLINT=false
PYLINT=false
DOXYGEN=false
BLD_DIR="osse-build"

rmfile () { if [ -f "$1" ]; then rm "$1"; fi }
rmdir () { if [ -d "$1" ]; then rm -r "$1"; fi }
cpout () { if [ -f "$1" ]; then cp "$1" ../; fi }

rmfile "compile_commands.json"

if [ -d $BLD_DIR ]; then
  cd $BLD_DIR
  make
else
  mkdir $BLD_DIR
  cd $BLD_DIR
  cmake ..
  make
fi

cpout apps/simple/simple.out
cpout apps/intermediate/intermediate.out
cpout apps/advanced/advanced.out
cpout apps/another/another.out
cpout compile_commands.json

cd ..

if $CPPLINT && [ -x "$(command -v cpplint)" ]; then \
  cpplint --quiet `find apps \
                   libs/collaborate/src/ \
                   libs/collaborate/include/collaborate/ \
                   libs/sgp4/src/ \
                   libs/sgp4/include/sgp4/ \
                   -name "*.h" \
                   -o \
                   -name "*.cpp"`
fi

if $PYLINT && [ -x "$(command -v pylint)" ]; then
  pylint --generated-members=gray \
         --disable=assignment-from-no-return \
         --disable=not-an-iterable \
         --disable=unsubscriptable-object \
         --disable=duplicate-code \
         --extension-pkg-whitelist=netCDF4 \
         `find util -name "*.py"`
fi

if $DOXYGEN && [ -x "$(command -v doxygen)" ]; then
  rmdir docs/man
  rmdir docs/cpp_html
  rmdir docs/refman
  rmdir docs/python_html
  rmfile docs/doxygen/log.txt
  if $VERBOSE; then
    doxygen docs/doxygen/cpp.txt
    doxygen docs/doxygen/py.txt
  else
    doxygen docs/doxygen/cpp.txt > docs/doxygen/log.txt
    doxygen docs/doxygen/py.txt > docs/doxygen/log.txt
    echo 'Doxygen logged to: "docs/doxygen/log.txt"'
  fi
fi
