#!/bin/sh

if [ -z "$1" ]
then
	minishell_path=../minishell
	minishell_dir_path=../
else
	minishell_path=$1
	minishell_dir_path=${minishell_path::-9}
fi

if [ -z "$2" ]
then
	test_dir=test_mandatory
else
	test_dir=$2
fi

red="\033[0;31m"
green="\033[0;32m"
reset="\033[0;39m"
test_li=$(ls ./$test_dir/*)

printf "path : $minishell_dir_path\n"
(cd $minishell_dir_path && make -s)
#LOOP FILL MISHELL OUTPUT
loop_test() {
	i=0
	for file in $test_li
	do
		printf "test : $file "
		cat $file | valgrind --log-fd=1 -q  --suppressions=readline_ignore.txt --leak-check=full  --show-leak-kinds=all $minishell_path 2>&- > ./minishell_output/minishell_output
		cat $file | bash  2>&- > ./expected_output/expected_output
		DIFF=$(diff ./expected_output/expected_output ./minishell_output/minishell_output)
		if [ "$DIFF" ]
		then
			printf ": $red KO $reset\n"
			printf "diff : $DIFF\n"
		else 
			printf ": $green OK\n"
		fi
		printf "$reset"
	done
}

loop_test
