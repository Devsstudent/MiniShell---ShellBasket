#!/bin/sh

minishell_path=$1
printf "ahahah : $minishell_path\n"
minishell_dir_path=${minishell_path::-9}
red="\033[0;31m"
green="\033[0;32m"
reset="\033[0;39m"
test_li=$(ls ./test_aa/*)

printf "path : $minishell_dir_path\n"
(cd $minishell_dir_path && make -s)
#LOOP FILL MISHELL OUTPUT
loop_test() {
	i=0
	for file in $test_li
	do
		printf "$file\n"
		cat $file | valgrind --log-fd=1 -q  --suppressions=readline_ignore.txt --leak-check=full  --show-leak-kinds=all $minishell_path 2>&- > /minishell_output/minishell_output
		cat $file | bash 2>&- > /expected_output/expected_output
		DIFF=$(diff /expected_output/expected_output /mininishell_output/minishell_output)
		if [ "$DIFF" ]
		then
			printf "$i: $red KO $reset\n"
			printf "diff : $DIFF\n"
		else 
			printf "\n$i: $green OK\n"
		fi
		printf "$reset"
	done
}

loop_test
