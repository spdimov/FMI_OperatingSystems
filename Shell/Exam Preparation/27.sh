#!/bin/bash
if [ $# -ne 2 ]; then
	echo "Wrong number of arguments"
	exit 1
fi

if [ ! -f $1 ]; then
	echo "First argument is not file"
	exit 2
fi

if [ ! -r $1 ]; then
	echo "File is not readable"
	exit 3
fi


unique_rows=$(mktemp)
cat $1 | cut -d',' -f 2- | sort | uniq > $unique_rows

while read line;do
	echo "------------$line"	
	count=$(grep -ic "[0-9]*,$line$" $1)
	if [[ $count -ne 1 ]];then
		grep "[0-9]*,$line$" <(cat $1 | sort -n ) | head -n 1 >> $2 
	else
		grep "[0-9]*,$line$" $1 >> $2
	fi

done < $unique_rows


