#!/bin/sh

set -xe

CCACHE=`which ccache 2> /dev/null`
CC="$CCACHE $CC"
CXX="$CCACHE $CXX"

mkdir -p obj/dep
$CC -o obj/dep/gifdec.o -std=c99 -O3 -I dep/gifdec/lib -Wno-deprecated-declarations -c dep/gifdec/src/gifdec.c


LIBS=opencv
LIBRARY_FLAGS=`pkgconf --cflags $LIBS`
CXXFLAGS="$LIBRARY_FLAGS -I dep/gifdec/lib -I lib -I src -Wno-unsequenced -std=c++17 -O3"
pkgconf openmp && CXXFLAGS="$CXXFLAGS -fopenmp"
LDFLAGS=`pkgconf --libs $LIBS`

mkdir -p obj/src
$CXX -o obj/src/ImageReader.o $CXXFLAGS -c src/ImageReader.cpp
$CXX -o obj/osb2mp4.o $CXXFLAGS -c osb2mp4.cpp

mkdir -p bin
$CXX -o bin/osb2mp4 $LDFLAGS `find obj -name "*.o"`
