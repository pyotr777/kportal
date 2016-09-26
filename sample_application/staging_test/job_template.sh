#!/bin/sh
#PJM --rsc-list "node=[NUMBER_OF_NODE]"
#PJM --rsc-list "elapse=[MAX_ELAPSE_TIME]"
#PJM -s
#PJM --stg-transfiles all
#PJM --stgin "./*.s.* ./"
#PJM --stgin "./[EXE_FILE] ./"
#PJM --stgin-dir "./[STAGE_IN_DIR] ./"
#PJM --stgout "./* ./"
#PJM --stgout-dir "./[STAGE_OUT_DIR] ./"
. /work/system/Env_base
echo "On node $(hostname -f)"
pwd
ls -l
filename="COMPNODETEST_$(hostname -d).txt"
echo "Creating file $filename"
touch $filename
echo "This file created on node: $(hostname -f)" >> $filename
echo "$(hostname -a)" >> $filename
ls -l

