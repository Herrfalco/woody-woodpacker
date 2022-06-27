#!/bin/bash

aes_test () {
	cd ./aes/tests
	make -s
	./aes_test
	make -s fclean
	cd ../..
}

lzw_test () {
	make -sC ./lzw
	lzw/lzw_test
	make -sC ./lzw fclean
}

if [ $# = 1 ]
then
	if [ $1 = 'aes' ]
	then
		aes_test
	elif [ $1 = 'lzw' ]
	then
		lzw_test
	else
		echo "Error: ./run_test.sh [aes/lzw]"
		exit
	fi
else
	aes_test
	lzw_test
fi

echo "------------------------------------"
