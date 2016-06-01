#!/bin/sh -x
#PJM --rsc-list "node=[NUMBER_OF_NODE]"
#PJM --rsc-list "elapse=[MAX_ELAPSE_TIME]"
#PJM --stg-transfiles all
#PJM --mpi "use-rankdir"
#PJM --stgin "rank=* ./[EXE_FILE] %r:./"
#PJM --stgin-dir "rank=* ./[STAGE_IN_DIR] %r:./"
#PJM --stgout-dir "rank=* ./ ./[STAGE_OUT_DIR]"
#PJM -s
#
. /work/system/Env_base
#
mpiexec ./[EXE_FILE] [EXE_ARGS]

