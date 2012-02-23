#!/bin/bash
export PATH="/home/dav/WORK/workspace/3rdparty/stlsoft-1.9.108/include:${PATH}"
#export PATH="/media/WORKSPACE/SVN/3rdparty/boost_1_47_0-bin/include:${PATH}"
#export PATH="/media/WORKSPACE/SVN/3rdparty/boost_1_47_0-bin/lib:${PATH}"


export PATH="/home/dav/xdg/include:${PATH}"
export PATH="/home/dav/xdg/lib:${PATH}"

cd debug
cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE:STRING=$1 ../
make
cd ../
