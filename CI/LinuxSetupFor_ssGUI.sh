#!/bin/bash

# This is for Ubuntu 22.04

set -e

apt update
apt upgrade -y

# Allow greater tls version compativility
apt install -y gnutls-bin

# Tools for building
apt install -y cmake
apt install -y g++

# XOrg and OpenGL
apt install -y mesa-common-dev
apt install -y mesa-utils
apt install -y xvfb
# apt install -y libx11-dev
apt install -y libxrandr-dev
apt install -y libxcursor-dev


# Run this on agent
#Xvfb :5 -screen 0 1280x720x24 &
#export DISPLAY=:5
#disown