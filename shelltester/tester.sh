#!/bin/sh

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


