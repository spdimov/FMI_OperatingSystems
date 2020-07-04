#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Invalid number of arguments"
	exit 1
fi

mkdir {a,b,c}
tmpfile=$(mktemp)

find . -type f | cut -c3- > tmpfile

while read p; do
	lines=$( wc -l "${p}" | cut -d' ' -f1 )
	if [ lines -lt $1 ] then
		mv "${p}" ./a/
	elif [ lines -gt $1 && lines -lt $2 ] then
		mv "{$p}" ./b/
	else mv "${p}" ./c/
	fi
done < tmpfile
