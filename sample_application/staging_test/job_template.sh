#!/bin/sh
#PJM --rsc-list "node=[NUMBER_OF_NODE]"
#PJM --rsc-list "elapse=[MAX_ELAPSE_TIME]"
#PJM -s
#PJM --stg-transfiles all
#PJM --stgin-dir "./staging_test ./"
#PJM --stgout "./* ./"
#PJM --stgout-dir "./stgoutdir ./"
. /work/system/Env_base
echo "On node $(hostname -f)"
pwd
ls -l
echo "Stage out dir is stgoutdir."
mkdir stgoutdir
touch stgoutdir/stgout_test.txt
filename="COMPNODETEST_$(hostname -d).txt"
echo "Creating file $filename"
touch $filename
echo "This file created on node: $(hostname -f)" >> $filename
echo "$(hostname -a)" >> $filename
ls -l

