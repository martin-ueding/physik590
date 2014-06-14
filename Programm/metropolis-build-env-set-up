#!/bin/bash
# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
# Licensed under The GNU Public License Version 2 (or later)

mkdir _build
pushd _build

mkdir Release
pushd Release

cmake -DCMAKE_BUILD_TYPE=Release ../..

popd

mkdir Debug
pushd Debug

cmake -DCMAKE_BUILD_TYPE=Debug ../..
