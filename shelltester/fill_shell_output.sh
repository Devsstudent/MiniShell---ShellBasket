#!/bin/sh

test_li=$(ls ./test_aa/*)
i=0
for file in $test_li
do
	cat $file | bash 2>&- > expected_output/qwe_$i
	i=$((i + 1))
done
