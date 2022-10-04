#!/bin/sh

minishell_path=../
red="\033[0;31m"
green="\033[0;32m"
reset="\033[0;39m"
test_li=$(ls ./test/our_test/*)
i=0
#LOOP FILL SHELL OUTPUT (1 time)
for file in $test_li
do
	cat $file | bash 2>&- > expected_output/qwe_$i
	i=$((i + 1))
done

i=0
#LOOP FILL MISHELL OUTPUT
for file in $test_li
do
	cat $file | bash 2>&- > minishell_output/qwe_$i
	i=$((i + 1))
done

i=0
#LOOP DIFF
for file in $(ls ./expected_output/*)
do
	DIFF=$(diff $file ./minishell_output/qwe_$i)
	printf "$DIFF"
	if [ $i -gt 25 ]
	then
		break
	fi
	if [ "$DIFF" ]
	then
		printf "$i: $red KO $reset\n"
		cat $file
	else
		printf "\n$i: $green OK\n"
	fi
	printf "$reset"
	i=$((i + 1))
done
