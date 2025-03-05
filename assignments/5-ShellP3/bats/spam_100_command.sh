#!/bin/bash

counter=1
num_runs=100

while [ $counter -le $num_runs ]; do
    echo Hello
    counter=$((counter + 1))
done
