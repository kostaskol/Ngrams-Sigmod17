#!/bin/bash

files=("small_static" "small_dynamic" "medium_static" "medium_dynamic")
prepath=$1

for file in ${files[@]}; do
	file=$prepath$file
	eval "build/ngrams -i $file.init -q $file.work > $file.output"
	diffs=$(diff $file.simple.result $file.output | wc -l)
	if [[ $diffs != 0 ]]; then
		echo "Problem at $file"
	else
		echo "$file OK"
	fi
done
