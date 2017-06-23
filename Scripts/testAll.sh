#!/bin/bash
AUX=""
#AUX="../"

#city="Oldenburg"
city="$4"

network="$AUX""data/""$city""/nodes.txt ""$AUX""data/""$city""/edges.txt"
qFile="$1""_""$2""_""$3"
queries="$AUX""data/""$city""/queries/""$qFile"".txt"

if [ ! -f "$queries" ]; then
    echo "Queries file not found!"
    echo "Usage: testAll.sh percX percY percT cityname"
    echo "with perc values between 0 and 100"
    exit
fi

declare -a SUFFIXES=("200" "400" "800" "1200" "1600" "2000")
for N in "${SUFFIXES[@]}"
do
	trajectories="$AUX""data/""$city""/trajectories/""$N"".dat"
	outFNR="$AUX""data/""$city""/output/FNR/""$qFile""-""$N"".txt"
	outX="$AUX""data/""$city""/output/X/""$qFile""-""$N"".txt"
	echo "----- Starting tests for ""$N"" objects.. -----"
	#echo "Executing.. FNR with ""$N"" objects.."
	./"$AUX"fnrtest.out $network $trajectories $queries $outFNR
	#echo "Executing..   X with ""$N"" objects.."
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
