#!/bin/bash

# Get GMP include and lib paths
GMP_INCLUDE=$(brew --prefix gmp)/include
GMP_LIB=$(brew --prefix gmp)/lib

# Compile the program
gcc -o main main.c -I"$GMP_INCLUDE" -L"$GMP_LIB" -lgmp

# Run the program
./main

