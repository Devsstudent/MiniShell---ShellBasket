#!/bin/sh

<<com
(cd ../bonus_dir && make -s)

#TEST 1
echo "cat | cat | yes | head | echo $?" | ../bonus_dir/minishell 2>&- > our_output/test1
echo "cat | cat | yes | head | echo $?" | bash 2>&- > original_output/test1


DIFF= $(cmp our_output/test1 original_output/test1)
if [ $DIFF ]
then
	printf "\033[31mKO\n";
else
	printf "\033[32mOK\n";
fi
com

minishell_path=../bonus_dir/minishell
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
	cat $file | valgrind --log-fd=1 -q  --suppressions=readline_ignore.txt --leak-check=full  --show-leak-kinds=all $minishell_path 2>&- > minishell_output/qwe_$i
	i=$((i + 1))
done

i=0
#LOOP DIFF
for file in $(ls ./expected_output/*)
do
	DIFF=$(diff $file ./minishell_output/qwe_$i)
	if [ $i -gt 25 ]
	then
		break
	fi
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
