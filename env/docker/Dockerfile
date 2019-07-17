FROM ubuntu:18.04

# Add some basic metadata.
LABEL version="0.0" maintainer="Ryan Linnabary" email="linnabary.24@osu.edu"

# Avoiding an Installation Prompt
ENV TZ=US/Eastern
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Setup
RUN apt update
RUN apt install -y --no-install-recommends apt-utils
RUN rm -rf /var/lib/apt/lists/*
RUN apt update
RUN apt install -y --no-install-recommends sssd libnss-sss
RUN rm -rf /var/lib/apt/lists/*
COPY nsswitch.conf /etc/nsswitch.conf
RUN chmod 0644 /etc/nsswitch.conf

# Linux Packages
RUN apt update
RUN apt install -y mpv
RUN apt install -y wget
RUN apt install -y cmake
RUN apt install -y make
RUN apt install -y doxygen
RUN apt install -y python3
RUN apt install -y libcanberra-gtk3-module
RUN apt install -y ffmpeg
RUN apt install -y libgeos-dev
RUN apt install -y graphviz
RUN apt install -y python3-tk
RUN apt install -y python3-pip
RUN apt install -y man
RUN apt install -y libproj-dev
RUN apt install -y proj-data
RUN apt install -y proj-bin
RUN apt install -y texlive-latex-extra
RUN apt install -y libnetcdf-dev

# Python Packages
RUN apt update
RUN pip3 install doxypypy
RUN pip3 install pylint
RUN pip3 install cpplint
RUN pip3 install scipy
RUN pip3 install cython
RUN pip3 install netcdf4
RUN pip3 install matplotlib
RUN pip3 install pypdf2
RUN pip3 install pandas
RUN pip3 install pillow
RUN pip3 install cartopy

# Create a location for source files
VOLUME /workspace
WORKDIR /workspace

# Run bash
CMD /bin/bash
