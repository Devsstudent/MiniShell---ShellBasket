#!/bin/sh

test_li=$(ls ./test_0/*)
i=0
for file in $test_li
do
	cat $file | bash 2>&- > expected_output/qwe_$i
	i=$((i + 1))
done
