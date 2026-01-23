#!/bin/bash
set -e

# Random seed for mutation engine (if used for compile-time variation)
MUTATION_SEED=$RANDOM

# Build for Windows (64-bit PE EXE)
x86_64-w64-mingw32-gcc -Os -DMUTATION_SEED=$MUTATION_SEED \
    src/*.c -o build/copilot365_agent_win.exe \
    -lws2_32 -ladvapi32

# Build for Linux (64-bit ELF)
gcc -Os -DMUTATION_SEED=$MUTATION_SEED \
    src/*.c -o build/copilot365_agent_linux
