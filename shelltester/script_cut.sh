#!/bin/bash

i=0
while read -r line
do
	echo "$line" > ./test/test_$i
	i=$((i + 1))
done < test_list
