#!/bin/bash

if [ $# -lt 1 ];then
	echo "Invalid arguments"
	exit 1
fi

if [ ! -d $1 ];then
        echo "First argument not directory"
 	exit 2
elif [ ! -r $1 ];then
	echo "Not readable directory"
	exit 3
fi

if [ $# -eq 2 ]; then
	find $1 -maxdepth 2 -type f -printf "%n %f\n" 2>/dev/null | awk -v min=$2 '{if($1>=min) print $2}'
elif [ $# -eq 1 ]; then
	find $1 -type l -exec file {} \; | grep broken | cut -d' ' -f1 | rev | cut -c2- | rev
fi

