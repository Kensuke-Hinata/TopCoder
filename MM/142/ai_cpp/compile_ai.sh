#!/bin/bash
echo "Compiling AI 1"
g++ StopTheElves_sullyper.cpp -std=c++11 -O3 -o ai1
echo "Compiling AI 2"
g++ StopTheElves_nika.cpp -std=c++11 -O3 -o ai2
echo "Compiling AI 3"
g++ StopTheElves_psyho.cpp -std=c++11 -O3 -o ai3
echo "Compiling AI 4"
g++ StopTheElves_nofto.cpp -std=c++11 -O3 -o ai4
echo "Done"