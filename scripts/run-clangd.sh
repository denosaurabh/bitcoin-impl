#!/bin/bash

# Get GMP include and lib paths
# export C_INCLUDE_PATH="/opt/homebrew/include:$C_INCLUDE_PATH"
# export LIBRARY_PATH="/opt/homebrew/lib:$LIBRARY_PATH"

# reload shell configuration
# source ~/.zshrc

# Compile the program
clang ../src/main.c -lgmp -o ../out/main 

# Run the program
../out/main

