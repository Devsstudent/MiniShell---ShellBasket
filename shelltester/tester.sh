#!/bin/sh

minishell_path=../bonus_dir/minishell
red="\033[0;31m"
green="\033[0;32m"
reset="\033[0;39m"
test_li=$(ls ./our_test/*)
(cd ../bonus_dir && make -s)

#LOOP FILL MISHELL OUTPUT
fill_minishell_output() {
	i=0
	printf "STARt\n"
	for file in $test_li
	do
		cat $file | valgrind --log-fd=1 -q  --suppressions=readline_ignore.txt --leak-check=full  --show-leak-kinds=all $minishell_path 2>&- > minishell_output/qwe_$i
		i=$((i + 1))
	done
	printf "END\n"
}

#LOOP DIFF

check_diff() {
i=0
printf "STARBIS\n"
for file in $(ls ./expected_output/*)
do
	printf "$file\n"
	DIFF=$(diff ./expected_output/qwe_$i ./minishell_output/qwe_$i)
	if [ "$DIFF" ]
	then
		printf "$i: $red KO $reset\n"
		printf "diff : $DIFF\n"
		printf "our_output : \n"
		cat ./minishell_output/qwe_$i
	else
		printf "\n$i: $green OK\n"
	fi
	printf "$reset"
	i=$((i + 1))
done
(cd ../bonus_dir && make -s fclean)
rm -rf minishell_output/*
printf "END_BIS\n"
}

fill_expected_output()
{
i=0
for file in $test_li
do
	cat $file | bash 2>&- > expected_output/qwe_$i
	i=$((i + 1))
done
}

fill_expected_output
fill_minishell_output
check_diff
