#!/bin/bash

export PATH=/media/WORKSPACE/github.com/qfm/workspace/root/install/bin:$PATH
export LD_LIBRARY_PATH=/media/WORKSPACE/github.com/qfm/workspace/root/install/lib

# $1 - Debug or Release
cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE:STRING=$1 ../workspace && 
make lvfs lvfs-core lvfs-db lvfs-arc && make -j10 qfm
