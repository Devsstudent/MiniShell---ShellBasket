#!/bin/sh

bash test > out
(cd .. && make)
../minishell
echo test
