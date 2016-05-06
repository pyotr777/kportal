#!/bin/bash
#PJM --rsc-list "node=3"
#PJM --rsc-list "elapse=00:01:00"
#PJM --stg-transfiles all
#PJM --mpi "use-rankdir"
#PJM --stgin "rank=* ./C_MPI.exe %r:./"
. /work/system/Env_base
mpiexec ./C_MPI.exe
