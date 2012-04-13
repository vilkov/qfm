#!/bin/bash

# Configuring and building
unset QTDIR
export PATH="$PWD/qtbase/bin:$PWD/qtrepotools/bin:$PATH"

./configure -developer-build -opensource -no-pch -no-gtkstyle -confirm-license -nomake examples -nomake tests -no-cups -no-javascript-jit -no-phonon-backend -no-accessibility
make -j4 module-qtbase module-qtwayland module-qttools module-qtdeclarative


# Cleaning
git submodule foreach --recursive 'git clean -dfx'


# Getting updates
git pull
git submodule update --recursive
