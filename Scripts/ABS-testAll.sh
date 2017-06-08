#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SCRIPT_DIR=${SCRIPT_DIR// /\\ }

network="$SCRIPT_DIR""/../data/Oldenburg/nodes.txt data/Oldenburg/edges.txt"
trajectories="$SCRIPT_DIR""/../data/Oldenburg/trajectories.dat"
queries="$SCRIPT_DIR""/../data/Oldenburg/queries.txt"
"$SCRIPT_DIR"/../fnrtest.out $network $trajectories $queries data/FNR-out.txt
"$SCRIPT_DIR"/../xtest.out $network $trajectories $queries data/X-out.txt