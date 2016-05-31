#!/bin/sh -x
#PJM --rsc-list "node=4"
#PJM --rsc-list "elapse=00:00:05"
#PJM --stg-transfiles all
#PJM --mpi "use-rankdir"
#PJM --stgin-dir ". ./" 
#PJM --stgout-dir "./ ./"
#PJM -s
. /work/system/Env_base
#
mpiexec a.out