#!/bin/bash -x
#
#PJM --rsc-list "node=[NUMBER_OF_NODE]"
#PJM --rsc-list "elapse=[MAX_ELAPSE_TIME]"
#PJM --stg-transfiles all
#PJM --stgin "./[EXE_FILE] ./"
#PJM --stgin-dir "./[STAGE_IN_DIR] ./"
#PJM --stgout-dir "./[STAGE_OUT_DIR] ./[STAGE_OUT_DIR]"
#PJM -s
#
. /work/system/Env_base
./[EXE_FILE] [EXE_ARGS]
