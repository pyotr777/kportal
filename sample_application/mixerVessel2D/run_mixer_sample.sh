#! /bin/sh -x
#PJM --rsc-list "node=[NUMBER_OF_NODE]"
#PJM --rsc-list "elapse=[MAX_ELAPSE_TIME]"
#PJM -s
#PJM --name job
#PJM --stgin-dir "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/platforms/S64FXFccDPOpt/lib ./lib"
#PJM --stgin-dir "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/platforms/S64FXFccDPOpt/lib/fjmpi ./lib"
#PJM --stgin-dir "/home/ra000007/a03106/OpenFOAM/ThirdParty-2.4.0/platforms/S64FXFccDPOpt/lib ./lib"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/platforms/S64FXFccDPOpt/lib/dummy/libmetisDecomp.so ./lib/"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/platforms/S64FXFccDPOpt/bin/blockMesh ./bin/"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/platforms/S64FXFccDPOpt/bin/topoSet ./bin/"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/platforms/S64FXFccDPOpt/bin/setsToZones ./bin/"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/platforms/S64FXFccDPOpt/bin/setFields ./bin/"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/platforms/S64FXFccDPOpt/bin/createPatch ./bin/"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/platforms/S64FXFccDPOpt/bin/interFoam ./bin/"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/platforms/S64FXFccDPOpt/bin/foamToVTK ./bin/"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/bin/tools/RunFunctions ./"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/etc/controlDict ./etc/"
#PJM --stgin "/home/ra000007/a03106/OpenFOAM/OpenFOAM-2.4.0/etc/cellModels ./etc/"
#PJM --stgin-dir "sample sample recursive=3"
#PJM --stgout-dir "sample sample recursive=3"
#
. /work/system/Env_base
#
export LD_LIBRARY_PATH=../lib:${LD_LIBRARY_PATH}
export PATH=../bin:${PATH}
export WM_PROJECT_SITE=../etc
export MPI_BUFFER_SIZE=20000000

cd sample

echo "Parameters for jobscript: $@"
# Change case parameters
if [[ "$#">0 ]]; then
	chomd +x ./change_case_pars.sh
	echo "Passing parameters: $@"
	./change_case_pars.sh "$@"
fi

. ../RunFunctions
echo "$(getApplication)"
# Set application name
application=`getApplication`

mv 0.org 0

chmod +x ./makeMesh

echo "In $(pwd)."
ls -l
runApplication ./makeMesh
cp 0/alpha.water.org 0/alpha.water
runApplication setFields
runApplication $application

echo "$(pwd)/.."
ls -l ..
# Convert to VTK
../bin/foamToVTK -ascii


