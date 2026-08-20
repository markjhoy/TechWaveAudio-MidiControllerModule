#!/bin/bash

#
# Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
#
# SPDX-License-Identifier: BSD-3-Clause
#
#

if [[ -e build ]]; then
  rm -rf build
fi;

if [[ -e dist ]]; then
  rm -rf dist
fi;

mkdir build

cd build || exit

cmake -DCMAKE_BUILD_TYPE=Debug -DPICO_DEOPTIMIZED_DEBUG=1 ..
cmake --build . --clean-first --target all -j 6 --config Debug

cd ..

mkdir dist
cd dist || exit

cp ../build/TechWaveAudio-MCM*.bin .
cp ../build/TechWaveAudio-MCM*.elf .
cp ../build/TechWaveAudio-MCM*.uf2 .

cd ..


