#!/bin/bash
#export PATH="/media/VIDEO/workspace/3rdparty/qt-bin/bin:${PATH}"
export PATH="/media/VIDEO/workspace/3rdparty/stlsoft/include:${PATH}"
export PATH="/media/VIDEO/workspace/modules/qtsingleinstance/dist/include:${PATH}"
export PATH="/media/VIDEO/workspace/modules/qtsingleinstance/dist/lib:${PATH}"
export PATH="/media/VIDEO/workspace/3rdparty/boost:${PATH}"

#export LDPATH="/media/VIDEO/workspace/modules/qtsingleinstance/dist/lib:${LDPATH}"
#export LD_LIBRARY_PATH="/media/VIDEO/workspace/modules/qtsingleinstance/dist/lib:${LD_LIBRARY_PATH}"

cd debug
cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE:STRING=$1 ../
make
cd ../