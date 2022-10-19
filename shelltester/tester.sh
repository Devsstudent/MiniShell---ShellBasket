#!/bin/sh

if [ -z "$1" ]
then
	minishell_path=../minishell
	minishell_dir_path=../
else
	minishell_path=$1
	minishell_dir_path=${minishell_path::-9}
fi

red="\033[0;31m"
green="\033[0;32m"
reset="\033[0;39m"
test_li=$(ls ./test_bonus/*)
cyan="\033[0;36m"
#Setup a directory with a lot a testfile inside like mine and give the dirpath of your choice behind
printf "$cyan     _______. __    __   _______  __       __      .___________. _______     _______.___________. _______ .______      \n"
printf "    /       ||  |  |  | |   ____||  |     |  |     |           ||   ____|   /       |           ||   ____||   _  \     \n"
printf "   |   (----\`|  |__|  | |  |__   |  |     |  |     \`---|  |----\`|  |__     |   (----\`---|  |----\`|  |__   |  |_)  |    \n"
printf "    \   \    |   __   | |   __|  |  |     |  |         |  |     |   __|     \   \       |  |     |   __|  |      /     \n"
printf ".----)   |   |  |  |  | |  |____ |  \`----.|  \`----.    |  |     |  |____.----)   |      |  |     |  |____ |  |\  \----.\n"
printf "|_______/    |__|  |__| |_______||_______||_______|    |__|     |_______|_______/       |__|     |_______|| _| \`._____|"
printf "$reset\n\n"

printf "path : $minishell_dir_path\n"
(cd $minishell_dir_path && make -s)
#LOOP FILL MISHELL OUTPUT
loop_test() {
	i=0
	for file in $test_li
	do
		printf "$file\n"
		cat $file | valgrind --log-fd=1 -q  --suppressions=readline_ignore.txt --leak-check=full  --show-leak-kinds=all $minishell_path 2>&- > ./minishell_output/minishell_output
		cat $file | bash  2>&- > ./expected_output/expected_output
		DIFF=$(diff ./expected_output/expected_output ./minishell_output/minishell_output)
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
