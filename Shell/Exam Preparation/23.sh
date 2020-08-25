#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid arguments"
	exit 1
fi

if [[ ! -r $1 || ! -d $1 ]]; then
	echo "First argument should be readable directory"
	exit 2
fi

find $1 -maxdepth 1 -type f -regex ".*vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-$2" 2>/dev/null | \
	awk -F'[-.]' ' BEGIN {max=-1} {if($3>max) max=$3; to_print=$0;} END{print to_print}'

