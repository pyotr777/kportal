#!/bin/bash
#PJM --rsc-list "rscgrp=small"
#PJM --rsc-list "node=1"
#PJM --rsc-list "elapse=00:01:00"
#PJM --stg-transfiles all
#PJM --stgin "./*.s.* ./"
#PJM --stgout "./* ./"
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

