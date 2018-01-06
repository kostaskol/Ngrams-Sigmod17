#!/bin/bash
git checkout giannis
./clean.sh -g
cd build
cmake ../CMakeLists.txt
make ngrams
mv ngrams ../../part3_wO3
cd ..

cd build
cmake ../CMakeLists2.txt
make ngrams
mv ngrams ../../part3
cd ..

git checkout part-2
./clean.sh -g
cd build
cmake ../CMakeLists.txt
make ngrams
mv ngrams ../../part2_wO3
cd ..

cd build
cmake ../CMakeLists2.txt
make ngrams
mv ngrams ../../part2
cd ..

execs=(../part3_wO3 ../part3 ../part2_wO3 ../part2)
files=(../part3_wO3.tests ../part3.tests ../part2_wO3.tests ../part2.tests)

for index in ${!execs[*]}
do
    echo "Getting the average execution time after 10 runs for each dataset" > ${files[$index]}
    echo " "
    echo "-----Small Dynamic-----" >> ${files[$index]}
    perf stat -r 10 -e task-clock -B ${execs[$index]} -i $smallD/small_dynamic.init -q $smallD/small_dynamic.work > /dev/null 2>>${files[$index]}
    echo "-----Small Dynamic with -p option-----" >> ${files[$index]}
    perf stat -r 10 -e task-clock -B ${execs[$index]} -i $smallD/small_dynamic.init -q $smallD/small_dynamic.work -p > /dev/null 2>>${files[$index]}
    echo "-----Medium Dynamic-----" >> ${files[$index]}
    perf stat -r 10 -e task-clock -B ${execs[$index]} -i $mediumD/medium_dynamic.init -q $mediumD/medium_dynamic.work > /dev/null 2>>${files[$index]}
    echo "-----Medium Dynamic with -p option-----" >> ${files[$index]}
    perf stat -r 10 -e task-clock -B ${execs[$index]} -i $mediumD/medium_dynamic.init -q $mediumD/medium_dynamic.work -p > /dev/null 2>>${files[$index]}
    echo "-----Large Dynamic-----" >> ${files[$index]}
    perf stat -r 10 -e task-clock -B ${execs[$index]} -i $largeD/large_dynamic.init -q $largeD/large_dynamic.work > /dev/null 2>>${files[$index]}
    echo "-----Large Dynamic with -p option-----" >> ${files[$index]}
    perf stat -r 10 -e task-clock -B ${execs[$index]} -i $largeD/large_dynamic.init -q $largeD/large_dynamic.work -p > /dev/null 2>>${files[$index]}
    echo " "
    echo "-----Small Static-----" >> ${files[$index]}
    perf stat -r 10 -e task-clock -B ${execs[$index]} -i $smallS/small_static.init -q $smallS/small_static.work > /dev/null 2>>${files[$index]}
    echo "-----Small Static with -p option-----" >> ${files[$index]}
    perf stat -r 10 -e task-clock -B ${execs[$index]} -i $smallS/small_static.init -q $smallS/small_static.work -p > /dev/null 2>>${files[$index]}
    echo "-----Medium Static-----" >> ${files[$index]}
    perf stat -r 10 -e task-clock -B ${execs[$index]} -i $mediumS/medium_static.init -q $mediumS/medium_static.work > /dev/null 2>>${files[$index]}
    echo "-----Medium Static with -p option-----" >> ${files[$index]}
    perf stat -r 10 -e task-clock -B ${execs[$index]} -i $mediumS/medium_static.init -q $mediumS/medium_static.work -p > /dev/null 2>>${files[$index]}
    echo "-----Large Static-----" >> ${files[$index]}
    perf stat -r 10 -e task-clock -B ${execs[$index]} -i $largeS/large_static.init -q $largeS/large_static.work > /dev/null 2>>${files[$index]}
    echo "-----Large Static with -p option-----" >> ${files[$index]}
    perf stat -r 10 -e task-clock -B ${execs[$index]} -i $largeS/large_static.init -q $largeS/large_static.work -p > /dev/null 2>>${files[$index]}
done
echo "Finished counting the average execution time :)"
