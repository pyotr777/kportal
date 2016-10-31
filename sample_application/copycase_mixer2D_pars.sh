#!/bin/bash -e

# Copy test case and change mesh parameters.
# Should be ran from OpenFOAM run directory.

# Macro "run" initialised with ". ~/OpenFOAM/OpenFOAM-2.4.0/etc/bashrc" and
# $export FOAM_RUN=~/OpenFOAM/run

cd ~/OpenFOAM/run/FoamCases

# Copy case to run directory
scp -r ~/OpenFOAM/OpenFOAM-2.4.0/tutorials/multiphase/interFoam/laminar/mixerVessel2D .


filename="mixerVessel2D/constant/polyMesh/blockMeshDict.m4"

# Case parameters
# Hub radius
r=0.2

# Impeller tip radius
rb=0.5

# Baffle tio radius
Rb=0.9

# Tank radius
R=1

# Mesh parameters
# Number of cells radially between hub and impeller tip
Nr=12

# Number of cells radially in each of the two regions between
# impeller and baffle tips
Ni=12

# Number of cells radially between baffle tip and tank
NR=12

# Number of cells azimuthally in each of the 8 blocks
Na=12

# Number of cells in the thickness of the slab
Nz=1

sed -r -i "s/define\(\s*r\s*,.*/define(r, $r)/" $filename
grep -C 1 "define(r," $filename
echo ""

sed -r -i "s/define\(\s*rb\s*,.*/define(rb, $rb)/" $filename
grep -C 1 "define(rb," $filename
echo ""

sed -r -i "s/define\(\s*Rb\s*,.*/define(Rb, $Rb)/" $filename
grep -C 1 "define(Rb," $filename
echo ""

sed -r -i "s/define\(\s*R\s*,.*/define(R, $R)/" $filename
grep -C 1 "define(R," $filename
echo ""

sed -r -i "s/define\(\s*Ni\s*,.*/define(Ni, $Ni)/" $filename
grep -C 1 "define(Ni," $filename
echo ""

printf "r    rb   Rb   R\n%3.1f  %3.1f  %3.1f  %3.1f\n     Ni\n     %3.1f\n" $r $rb $Rb $R $Ni > parameters.txt
