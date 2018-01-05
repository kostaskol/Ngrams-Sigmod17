#!/bin/bash

echo "Small Dynamic parallel" > $1
/usr/bin/time -f "\t%E real" -a -o $1 build/ngrams -i /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/small/small_dynamic/small_dynamic.init -q /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/small/small_dynamic/small_dynamic.work -p > /dev/null
echo "---------------------------------------" >> $1
echo >> $1

echo "Medium Dynamic parallel" >> $1
/usr/bin/time -f "\t%E real" -a -o $1 build/ngrams -i /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/medium/medium_dynamic/medium_dynamic.init -q /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/medium/medium_dynamic/medium_dynamic.work -p > /dev/null
echo "---------------------------------------" >> $1
echo >> $1

echo "Large Dynamic parallel" >> $1
/usr/bin/time -f "\t%E real" -a -o $1 build/ngrams -i /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/large/large_dynamic/large_dynamic.init -q /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/large/large_dynamic/large_dynamic.work -p > /dev/null
echo "---------------------------------------" >> $1
echo >> $1

echo "------------------------------------------------------------------------------" >> $1
echo >> $1

echo "Small Dynamic" >> $1
/usr/bin/time -f "\t%E real" -a -o $1 build/ngrams -i /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/small/small_dynamic/small_dynamic.init -q /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/small/small_dynamic/small_dynamic.work > /dev/null
echo "---------------------------------------" >> $1
echo >> $1

echo "Medium Dynamic" >> $1
/usr/bin/time -f "\t%E real" -a -o $1 build/ngrams -i /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/medium/medium_dynamic/medium_dynamic.init -q /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/medium/medium_dynamic/medium_dynamic.work > /dev/null
echo "---------------------------------------" >> $1
echo >> $1

echo "Large Dynamic" >> $1
/usr/bin/time -f "\t%E real" -a -o $1 build/ngrams -i /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/large/large_dynamic/large_dynamic.init -q /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/large/large_dynamic/large_dynamic.work > /dev/null
echo "---------------------------------------" >> $1
echo >> $1

echo "------------------------------------------------------------------------------" >> $1
echo >> $1

echo "Small Static parallel" >> $1
/usr/bin/time -f "\t%E real" -a -o $1 build/ngrams -i /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/small/small_static/small_static.init -q /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/small/small_static/small_static.work -p > /dev/null
echo "---------------------------------------" >> $1
echo >> $1

echo "Medium Static parallel" >> $1
/usr/bin/time -f "\t%E real" -a -o $1 build/ngrams -i /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/medium/medium_static/medium_static.init -q /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/medium/medium_static/medium_static.work -p > /dev/null
echo "---------------------------------------" >> $1
echo >> $1

echo "Large Static parallel" >> $1
/usr/bin/time -f "\t%E real" -a -o $1 build/ngrams -i /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/large/large_static/large_static.init -q /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/large/large_static/large_static.work -p > /dev/null
echo "---------------------------------------" >> $1
echo >> $1

echo "------------------------------------------------------------------------------" >> $1
echo >> $1

echo "Small Static" >> $1
/usr/bin/time -f "\t%E real" -a -o $1 build/ngrams -i /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/small/small_static/small_static.init -q /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/small/small_static/small_static.work > /dev/null
echo "---------------------------------------" >> $1
echo >> $1

echo "Medium Static" >> $1
/usr/bin/time -f "\t%E real" -a -o $1 build/ngrams -i /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/medium/medium_static/medium_static.init -q /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/medium/medium_static/medium_static.work > /dev/null
echo "---------------------------------------" >> $1
echo >> $1

echo "Large Static" >> $1
/usr/bin/time -f "\t%E real" -a -o $1 build/ngrams -i /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/large/large_static/large_static.init -q /home/Dropbox/SKATA/Project_Ioannidis/test_dataset/part2-3/large/large_static/large_static.work > /dev/null
echo "---------------------------------------" >> $1
echo >> $1
