#!/bin/bash
g++ -std=c++17 heart.cpp -o heart.out
./heart.out $1
python3 draw.py $1 "uniform"
python3 draw.py $1 "chord"
