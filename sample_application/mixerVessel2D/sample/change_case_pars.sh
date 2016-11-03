#!/bin/bash

# Change OpenFOAM case parameters.
# For MixerVessel2D case.

read -rd '' USAGE <<EOF
	Change mesh parameters. Should be ran from case directory.
	Usage:
	$(basename $0) [-r hub radius (x10 cm)] [-rb Impeller tip radius] [-Rb Baffle tip radius] [-R Tank raidus]
	 [-t Simulation time (s)] [-s Rotation speed (rps)]
	 [-e email address for sending simulation video]
EOF

echo "$0"

if [[ "$#"<1 ]]; then
	echo "$USAGE"
	exit 1
else
	echo "Got parameters: $@"
fi

filename="constant/polyMesh/blockMeshDict.m4"
controldict="system/controlDict"
fvoptions="constant/fvOptions"

# Default values
# Hub radius
r=0.2
# Impeller tip radius
rb=0.5
# Baffle tip radius
Rb=0.7
# Tank radius
R=1
# Simulation end time (s)
endT=6
# Rotation speed
omega=6.2831853

# Mesh parameters
# Number of cells radially between hub and impeller tip
Nr=12
# Number of cells radially in each of the two regions between
# impeller and baffle tips
Ni=4

# Number of cells radially between baffle tip and tank
NR=12

# Number of cells azimuthally in each of the 8 blocks
Na=12

# Number of cells in the thickness of the slab
Nz=1

pi=3.14159265359

email=peter@stair.center

while test $# -gt 0; do
	case "$1" in
		-r)
			r=$2;shift;
			sed -r -i "s/define\(\s*r\s*,.*/define(r, $r)/" $filename;
			grep -C 1 "define(r," $filename;
			echo "";
			;;
		-rb)
			rb=$2;shift;
			sed -r -i "s/define\(\s*rb\s*,.*/define(rb, $rb)/" $filename;
			grep -C 1 "define(rb," $filename;
			echo "";
			;;
		-Rb)
			Rb=$2;shift;
			sed -r -i "s/define\(\s*Rb\s*,.*/define(Rb, $Rb)/" $filename;
			grep -C 1 "define(Rb," $filename;
			echo "";
			;;
		-R)
			R=$2;shift;
			sed -r -i "s/define\(\s*R\s*,.*/define(R, $R)/" $filename;
			grep -C 1 "define(R," $filename;
			echo "";
			;;
		-t)
			endT=$2;shift;
			sed -r -i "s/^endTime\s*.*/endTime\t\t$endT;/" $controldict;
			grep "^endTime" $controldict;
			echo "";
			;;
		-s)
			omega=$(awk "BEGIN {print $pi*$2}");shift;
			sed -r -i "s/\s*omega\s*constant\s*.*/        omega     constant $omega;/" $fvoptions;
			grep "omega" $fvoptions;
			echo "";
			;;
		-e)
			email=$2;shift;
			echo "export email=\"$2\"" > sample/email.source
		--)
			shift
			break;;
		-*)
			echo "Invalid option: $1"
			echo "$USAGE"
			exit 1;;
		*)
			echo "Invalid option: $1"
			echo "$USAGE"
			exit 1;;
	esac
	shift
done



printf "Mixer vessel configuration (distance from center, x10 cm):\n%3.1f  %3.1f  %3.1f  %3.1f\nRotation speed (rps):  %f\nSimulation time (s):  %3.1f\n\n" $r $rb $Rb $R $endT $omega > parameters.txt
