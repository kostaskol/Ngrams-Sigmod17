#!/bin/bash

if [ -z "$1" ]; then
    rm -rf build/*
elif [ "$1" == "-g" ]; then
    rm -rf build/CMakeFiles build/cmake_install* build/Makefile build/CMakeCache.txt
else
    echo "Failed to clean"
fi
