#!/bin/sh
#PJM --rsc-list "node=[NUMBER_OF_NODE]"
#PJM --rsc-list "elapse=[MAX_ELAPSE_TIME]"
#PJM -s
#PJM --name job
#PJM --stgin-dir "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/platforms/S64FXFccDPOpt/lib ./lib"
#PJM --stgin-dir "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/platforms/S64FXFccDPOpt/lib/dummy ./lib"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/platforms/S64FXFccDPOpt/bin/blockMesh ./bin/"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/platforms/S64FXFccDPOpt/bin/setFields ./bin/"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/platforms/S64FXFccDPOpt/bin/interFoam ./bin/"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/bin/tools/RunFunctions ./"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/etc/controlDict ./etc/"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/etc/cellModels ./etc/"
#PJM --stgin-dir "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/tutorials/multiphase/interFoam/laminar/damBreak ./damBreak recursive=3"
#PJM --stgout-dir "./damBreak ./damBreak.out recursive=3"
#
. /work/system/Env_base
#
export LD_LIBRARY_PATH=../lib:${LD_LIBRARY_PATH}
PATH=../bin:${PATH}
export WM_PROJECT_SITE=../etc
echo "Diagnostic info:"
echo "Env:"
env | grep -i openfoam
echo "PWD: $(pwd)"
echo "Files:"
find .

cd damBreak
. ../RunFunctions
mv 0/alpha.water.org 0/alpha.water
rm log.blockMesh
rm log.setFields
rm log.interFoam

runApplication blockMesh
runApplication setFields
runApplication interFoam
echo "--- Job finished ---"
echo "Files:"
find .

