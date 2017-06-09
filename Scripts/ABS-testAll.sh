#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SCRIPT_DIR=${SCRIPT_DIR// /\\ }

AUX="$SCRIPT_DIR/../"

network="$AUX""data/Oldenburg/nodes.txt ""$AUX""data/Oldenburg/edges.txt"
queries="$AUX""data/Oldenburg/queries.txt"

declare -a SUFFIXES=("200" "400" "800" "1200" "1600" "2000")
for N in "${SUFFIXES[@]}"
do
	trajectories="$AUX""data/Oldenburg/trajectories/""$N"".dat"
	outFNR="$AUX""data/Oldenburg/output/FNR/""$N"".txt"
	outX="$AUX""data/Oldenburg/output/X/""$N"".txt"
	echo "Executing.. FNR with ""$N"" objects.."
	time "$AUX"fnrtest.out $network $trajectories $queries $outFNR
	echo "Executing.. X with ""$N"" objects.."
	time "$AUX"xtest.out $network $trajectories $queries $outX

	echo ""; echo "Checking differences between Output files.."
	if diff $outFNR $outX >/dev/null; then
		printf "Passed\n"
	else
		printf "Failed\n"
		sdiff $outFNR $outX
	fi
	echo "-------------------------------------------"; echo ""
done