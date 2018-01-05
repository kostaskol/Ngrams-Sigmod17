#!/bin/bash

if [ -z "$1" ]; then
    rm -rf build/*
elif [ "$1" == "-g" ]; then
    rm -rf build/CMake* build/cmake* build/Makefile
else
    echo "Failed to clean"
fi
