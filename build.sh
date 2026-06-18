#!/bin/bash

if [[ -e build ]]; then
  rm -rf build
fi;

if [[ -e dist ]]; then
  rm -rf dist
fi;

mkdir build

cd build

cmake ..
make

cd ..

mkdir dist
cd dist

cp ../build/TechWaveAudio-MCM*.bin .
cp ../build/TechWaveAudio-MCM*.elf .
cp ../build/TechWaveAudio-MCM*.uf2 .

cd ..


