#!/bin/bash

cd glpk-4.40/
./configure
make -j4
cd ..
./configure --with-boost=/home/macmanes/boost_1_60_0/build/
make -j4
echo PATH=$PATH:$(pwd) >> ~/.profile
