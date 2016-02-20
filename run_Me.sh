#!/bin/bash -ve

### run BinPacker for paired-end reads ###

./BinPacker -s fq -p pair -l ./sample_test/reads.left.fq -r ./sample_test/reads.right.fq -m RF -k 25 -g 200 

### run BinPacker for single-end reads ###

#./BinPacker -s fq -p single -u ./sample_test/reads.left.fq -k 25

##### Done #####



