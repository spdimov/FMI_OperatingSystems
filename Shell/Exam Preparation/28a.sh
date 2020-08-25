#!/bin/bash

numbers=$(mktemp)
cat | egrep "^[+-]?[0-9]+$" | sort -n > $numbers

min=$(cat $numbers | head -n 1)
max=$(cat $numbers | tail -n 1)

abs_max=$(sed 's/-//' <(echo $max))
abs_min=$(sed 's/-//' <(echo $min))

if [ $abs_max -ge $abs_min ];then
	echo "dada"
	egrep "^[+-]?${abs_max}$" $numbers | sort -n | uniq
else 
	egrep "^[+-]?${abs_min}$" $numbers | sort -n | uniq
fi
