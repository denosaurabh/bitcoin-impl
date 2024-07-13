#!/bin/bash

# Ensure the output directory exists
mkdir -p ../out

# Generate compile_commands.json and compile the project
bear -- gcc \
        -I/usr/local/include \
        -L/usr/local/lib \
        -I/opt/homebrew/bin/openssl/include \
        -lssl \
        -lcrypto \
        -lgmp \
        -o ../out/main ../src/*.c

# Run the program
../out/main
