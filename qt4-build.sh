#!/bin/bash

# Configuring and building
export QTDIR=/media/BACKUP/qt-everywhere-opensource-src-4.8.1-bin
export PATH="/media/BACKUP/qt-everywhere-opensource-src-4.8.1-bin/bin:$PATH"

/media/BACKUP/qt-everywhere-opensource-src-4.8.1/configure \
  -debug \
  -developer-build \
  -opensource \
  -no-pch \
  -no-gtkstyle \
  -confirm-license \
  -nomake examples \
  -nomake tests \
  -no-cups \
  -no-webkit \
  -no-qt3support \
  -no-phonon \
  -no-multimedia \
  -no-script \
  -no-scripttools \
  -no-javascript-jit \
  -no-phonon-backend \
  -no-accessibility &&
make -j4
