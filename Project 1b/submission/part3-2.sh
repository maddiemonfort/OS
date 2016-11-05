#!/bin/sh
./safe_increment 100 safe.txt 0 pids.txt &
PID_A=$!

./safe_increment 100 safe.txt 1 pids.txt &
PID_B=$!

echo $PID_A $PID_B > pids.txt

wait

if [ -z "$( ./consecutive safe.txt )" ]; then
    echo -e "\033[01;36mTest Passed\033[0m"
else
    echo -e "\033[01;31mTest Failed\033[0m"
fi
