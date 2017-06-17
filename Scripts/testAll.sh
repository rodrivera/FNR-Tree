#!/bin/bash
AUX=""
#AUX="../"

network="$AUX""data/Oldenburg/nodes.txt ""$AUX""data/Oldenburg/edges.txt"
qFile="$1""_""$2""_""$3"
queries="$AUX""data/Oldenburg/queries/""$qFile"".txt"

if [ ! -f "$queries" ]; then
    echo "Queries file not found!"
    echo "Usage: testAll.sh percX percY percT"
    echo "with perc values between 0 and 1"
    exit
fi

declare -a SUFFIXES=("200" "400" "800" "1200" "1600" "2000")
for N in "${SUFFIXES[@]}"
do
	trajectories="$AUX""data/Oldenburg/trajectories/""$N"".dat"
	outFNR="$AUX""data/Oldenburg/output/FNR/""$qFile""-""$N"".txt"
	outX="$AUX""data/Oldenburg/output/X/""$qFile""-""$N"".txt"
	echo "Executing.. FNR with ""$N"" objects.."
	./"$AUX"fnrtest.out $network $trajectories $queries $outFNR
	echo "Executing.. X with ""$N"" objects.."
	./"$AUX"xtest.out $network $trajectories $queries $outX

	echo ""; echo "Checking differences between Output files.."
	if diff $outFNR $outX >/dev/null; then
		printf "Passed\n"
	else
		printf "¡¡ Failed !!\n"
		#sdiff $outFNR $outX
		exit
	fi
	echo "-------------------------------------------"; echo ""
done
