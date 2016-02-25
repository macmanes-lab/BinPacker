## BinPacker 
forked from https://sourceforge.net/projects/transcriptomeassembly/

#### To Install
1. modify install.sh such that it reflects the location of *your* boost install
2. `./install.sh`

#### To run

```
BinPacker -d -s fq -p pair -l ./sample_test/reads.left.fq -r ./sample_test/reads.right.fq -m RF -k 25 -g 200 -o test
```
