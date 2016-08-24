#! /bin/bash
#PJM --rsc-list "rscgrp=small"
#PJM --rsc-list "node=2x3x2"
#PJM --rsc-list "elapse=00:25:00"
#PJM --stg-transfiles "all"
#PJM --mpi "use-rankdir"
#PJM --stgin "rank=* ./matmult.exe %r:./"
env
. /work/system/Env_base
mpiexec ./matmult.exe
