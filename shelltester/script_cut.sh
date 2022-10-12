#!/bin/bash
line=test_list_bis

i=0
while read -r line
do
	printf "$line"
	echo "$line" > ./test_0/test_$i
	i=$((i + 1))
done < test_list_bis
