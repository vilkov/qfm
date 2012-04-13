#!/bin/bash
export PATH="/home/dav/WORK/workspace/3rdparty/stlsoft-1.9.108/include:${PATH}"
#export PATH="/media/WORKSPACE/SVN/3rdparty/boost_1_47_0-bin/include:${PATH}"
#export PATH="/media/WORKSPACE/SVN/3rdparty/boost_1_47_0-bin/lib:${PATH}"


# Qt5
#unset QTDIR
#export LD_LIBRARY_PATH="/media/BACKUP/qt5/qtbase/lib"
#export PATH="/media/BACKUP/qt5/qtbase/include:${PATH}"
#export PATH="/media/BACKUP/qt5/qtbase/lib:${PATH}"
#export PATH="/media/BACKUP/qt5/qtbase/bin:/media/BACKUP/qt5/qtrepotools/bin:${PATH}"


export PATH="/home/dav/xdg/include:${PATH}"
export PATH="/home/dav/xdg/lib:${PATH}"

cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE:STRING=$1 ../
make
