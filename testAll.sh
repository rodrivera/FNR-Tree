#!/bin/bash
network="data/Oldenburg/nodes.txt data/Oldenburg/edges.txt"
trajectories="data/Oldenburg/trajectories.dat"
queries="data/Oldenburg/queries.txt"
./fnrtest.out $network $trajectories $queries data/FNR-out.txt
./xtest.out $network $trajectories $queries data/X-out.txt