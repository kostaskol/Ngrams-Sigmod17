#!/bin/bash

execs=(../part3)                    #check this first
files=(../part3-no_clean.tests)     #check this after

for index in ${!execs[*]}
do
    echo "Getting the average execution time after 10 runs for each dataset" > ${files[$index]}
    echo " "
    echo "-----Small Dynamic-----" >> ${files[$index]}
    perf stat -r 1 -e task-clock -B ${execs[$index]} -i $smallD/small_dynamic.init -q $smallD/small_dynamic.work > /dev/null 2>>${files[$index]}

    if [[ $1 -eq "-p" ]]; then
        echo "-----Small Dynamic with -p option-----" >> ${files[$index]}
        perf stat -r 1 -e task-clock -B ${execs[$index]} -i $smallD/small_dynamic.init -q $smallD/small_dynamic.work -p > /dev/null 2>>${files[$index]}
    fi

    echo "-----Medium Dynamic-----" >> ${files[$index]}
    perf stat -r 1 -e task-clock -B ${execs[$index]} -i $mediumD/medium_dynamic.init -q $mediumD/medium_dynamic.work > /dev/null 2>>${files[$index]}

    if [[ $1 -eq "-p" ]]; then
        echo "-----Medium Dynamic with -p option-----" >> ${files[$index]}
        perf stat -r 1 -e task-clock -B ${execs[$index]} -i $mediumD/medium_dynamic.init -q $mediumD/medium_dynamic.work -p > /dev/null 2>>${files[$index]}
    fi

    echo "-----Large Dynamic-----" >> ${files[$index]}
    perf stat -r 1 -e task-clock -B ${execs[$index]} -i $largeD/large_dynamic.init -q $largeD/large_dynamic.work > /dev/null 2>>${files[$index]}

    if [[ $1 -eq "-p" ]]; then
        echo "-----Large Dynamic with -p option-----" >> ${files[$index]}
        perf stat -r 1 -e task-clock -B ${execs[$index]} -i $largeD/large_dynamic.init -q $largeD/large_dynamic.work -p > /dev/null 2>>${files[$index]}
    fi

    echo " "
    echo "-----Small Static-----" >> ${files[$index]}
    perf stat -r 1 -e task-clock -B ${execs[$index]} -i $smallS/small_static.init -q $smallS/small_static.work > /dev/null 2>>${files[$index]}

    if [[ $1 -eq "-p" ]]; then
        echo "-----Small Static with -p option-----" >> ${files[$index]}
        perf stat -r 1 -e task-clock -B ${execs[$index]} -i $smallS/small_static.init -q $smallS/small_static.work -p > /dev/null 2>>${files[$index]}
    fi

    echo "-----Medium Static-----" >> ${files[$index]}
    perf stat -r 1 -e task-clock -B ${execs[$index]} -i $mediumS/medium_static.init -q $mediumS/medium_static.work > /dev/null 2>>${files[$index]}

    if [[ $1 -eq "-p" ]]; then
        echo "-----Medium Static with -p option-----" >> ${files[$index]}
        perf stat -r 1 -e task-clock -B ${execs[$index]} -i $mediumS/medium_static.init -q $mediumS/medium_static.work -p > /dev/null 2>>${files[$index]}
    fi

    echo "-----Large Static-----" >> ${files[$index]}
    perf stat -r 1 -e task-clock -B ${execs[$index]} -i $largeS/large_static.init -q $largeS/large_static.work > /dev/null 2>>${files[$index]}

    if [[ $1 -eq "-p" ]]; then
        echo "-----Large Static with -p option-----" >> ${files[$index]}
        perf stat -r 1 -e task-clock -B ${execs[$index]} -i $largeS/large_static.init -q $largeS/large_static.work -p > /dev/null 2>>${files[$index]}
    fi

done
echo "Finished counting the average execution time :)"
