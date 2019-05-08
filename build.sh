#!/bin/bash
cd Builds/LinuxMakefile
make
cp build/MusicBox ../../MusicBox
cd ../../
./MusicBox
