#!/bin/bash

temp=$(mktemp)

numbers=$(mktemp)
cat $1 | egrep "^[+-]?[0-9]+$" | sort -nr > $numbers
max_sum=0
while read number; do
	sum=0
	num_copy=$number
	number=$(sed 's/-//' <(echo $number))
	while [ $number -ne 0 ];do
		last_digit=$(($number % 10))
		sum=$(($sum+$last_digit))
		number=$(($number / 10))
	done
	
	if [ $sum -gt $max_sum ];then
		max_sum=$sum
	fi
	
	echo "$num_copy $sum" >> $temp
done < $numbers

numbers_with_max_sum=$(mktemp)
egrep "^[+-]?[0-9]+ $max_sum" $temp > $numbers_with_max_sum
sort -n -t' ' -k1 $numbers_with_max_sum | head -n 1 | cut -d' ' -f1
