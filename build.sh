#!/bin/bash

# $1 - Debug or Release
cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE:STRING=$1 ../workspace &&
make qfm
