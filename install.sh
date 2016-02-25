#!/bin/bash

cd glpk-4.40/
./configure
make -j4
cd ..
./configure --with-boost=/home/macmanes/boost_1_60_0/build/
make -j4
cd plugins
git clone https://github.com/lh3/seqtk.git
cd seqtk
make -j4
cd ../../
echo PATH=$PATH:$(pwd):$(pwd)/plugins/seqtk >> ~/.profile
echo $(pwd)
BinPacker -s fq -p pair -l ./sample_test/reads.left.fq -r ./sample_test/reads.right.fq -m RF -k 25 -g 200 -o test -d
rm -fr test
