#!/bin/bash

if [ $# -ne 2 ];then
	echo "Invalid arguments"
	exit 1
fi

if [[ ! -f $1 || ! -r $1 ]];then
	echo "First argument is not readable file"
	exit 2
fi

if [[ ! -d $2 || ! -r $2 ]];then
	echo "Second argument is not readable directory"
	exit 3
fi

touch "$2/dict.txt"

unique_number=1

while read line;do
	name=$(cut -d':' -f1 <(echo $line | tr -s ' ') | cut -d' ' -f1,2)
	count_found=$(grep -ic "$name" $2/dict.txt)
	if [ $count_found -eq 0 ];then
		echo "$name; $unique_number" >> $2/dict.txt
		echo $line >> $2/$unique_number.txt
		unique_number=$(($unique_number+1))
	else
		correct_number=$(grep "$name" $2/dict.txt | cut -d';' -f2 | tr -d ' ')
		echo $line >> $2/"${correct_number}".txt
	fi
	

done < $1
