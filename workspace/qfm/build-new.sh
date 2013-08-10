#!/bin/bash
export PATH="/media/WORKSPACE/SVN/3rdparty/stlsoft-1.9.108/include:${PATH}"
export PATH="/media/WORKSPACE/SVN/3rdparty/boost_1_47_0-bin/include:${PATH}"
export PATH="/media/WORKSPACE/SVN/3rdparty/boost_1_47_0-bin/lib:${PATH}"

export PATH="/home/dav/xdg/include:${PATH}"
export PATH="/home/dav/xdg/lib:${PATH}"


#export LDPATH="/media/VIDEO/workspace/modules/qtsingleinstance/dist/lib:${LDPATH}"
#export LD_LIBRARY_PATH="/media/VIDEO/workspace/modules/qtsingleinstance/dist/lib:${LD_LIBRARY_PATH}"

cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE:STRING=$1 ../
make
