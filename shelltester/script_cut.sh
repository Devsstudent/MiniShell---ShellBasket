#!/bin/bash

i=0
while read -r line
do
	printf "$line"
	echo "$line" > ./test_bonus/$i\__test
	i=$((i + 1))
done < test_list_bonus
