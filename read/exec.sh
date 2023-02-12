#!/bin/bash

# $1: Path of file to read
# $2: Read offset
# $3: Read length
# $4: Number of iterations

if [ -z "$4" ]; then
    echo "Not enough parameters"
    exit 1
fi

echo 3 > /proc/sys/vm/drop_caches
echo "Start time (ns): [$(date +%s%N)]"
for idx in {1..$4}
do
    echo "Iterating #$idx"
    ./read.out $1 $2 $3
done

echo "End   time (ns): [$(date +%s%N)]"
