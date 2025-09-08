#!/usr/bin/env bash

##
# docker-entrypoint.sh
#
# Copyright (c) 2001-2025 Daniel Horn, pyramid3d, Stephen G. Tuggy,
# and other Vega Strike Contributors
#
# https://github.com/vegastrike/Vega-Strike-Engine-Source
#
# This file is part of Vega Strike.
#
# Vega Strike is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Vega Strike is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Vega Strike.  If not, see <https://www.gnu.org/licenses/>.
#


set -e

echo "-----------------------------------------"
echo "--- docker-entrypoint.sh | 2025-06-27 ---"
echo "-----------------------------------------"

#----------------------------------
# validate parameters
#----------------------------------

while [ $# -gt 0 ]; do
  case "$1" in
    --preset_name=*)
      preset_name="${1#*=}"
      ;;
    --preset-name=*)
      preset_name="${1#*=}"
      ;;
    --build_type=*)
      build_type="${1#*=}"
      ;;
    --build-type=*)
      build_type="${1#*=}"
      ;;
    *)
      printf "***************************\n"
      printf "* Error: Invalid argument.*\n"
      printf "***************************\n"
      exit 1
  esac
  shift
done

echo "Re-run bootstrap"
script/bootstrap

if [ "$COMPILER" == "gcc" ]
then
  export CC=gcc
  export CXX=g++
elif [ "$COMPILER" == "clang" ]
then
  export CC=clang
  export CXX=clang++
fi

if [ -z "$preset_name" ] && [ -n "$PRESET_NAME" ]
then
  preset_name="${PRESET_NAME}"
fi

script/build --preset_name="${preset_name}"

if [ $IS_RELEASE -eq 1 ]
then
  script/package --preset_name="${preset_name}"
else
  script/test --preset_name="${preset_name}"
fi
