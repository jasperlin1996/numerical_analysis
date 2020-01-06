#!/bin/bash
# Usage: bash compile.sh data.txt
g++ -std=c++17 horner.cpp -o horner.out
./horner.out $1
python3 draw.py $1 "uniform"
python3 draw.py $1 "chord"
