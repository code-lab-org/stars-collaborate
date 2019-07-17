#!/bin/bash
#
# Copyright (C) 2019 Ryan Linnabary
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

HOSTNAME="collaborate-test"
uid="$(id -u)"
gid="$(id -g)"

if [[ "$DISPLAY" = *localhost* ]]; then
  # SSH Session
  if [[ -z $XAUTHORITY ]]; then XAUTHORITY="$HOME/.Xauthority"; fi
  backup="$(dirname "$XAUTHORITY")/.Xauthority.docker.orig"
  cp "$XAUTHORITY" "$backup"
  xauth list | while read -r host type key; do
    if [[ "$host" = *$(hostname)* ]]; then
      xauth add ${host/$(hostname)/$HOSTNAME} $type $key
    fi
  done
else
  # Local
  xhost local:
fi

docker run \
       -it \
       --user "$uid:$gid" \
       --hostname "$HOSTNAME" \
       --network="host" \
       -e HOME="/workspace" \
       -e DISPLAY="$DISPLAY" \
       -e XAUTHORITY="/workspace/.Xauthority" \
       -e MANPATH="${MANPATH}:/workspace/collaborate/docs/man/" \
       -e PYTHONPATH="${PYTHONPATH}:/workspace/collaborate/util/" \
       -v /var/lib/sss/pipes/:/var/lib/sss/pipes/:rw \
       -v /var/lib/sss/mc/:/var/lib/sss/mc/:ro \
       -v /tmp/.X11-unix:/tmp/.X11-unix \
       -v "$HOME":/workspace \
       -w /workspace/collaborate \
       collaborate
