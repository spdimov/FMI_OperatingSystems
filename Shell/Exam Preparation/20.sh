#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Invalid arguments"
	exit 1
fi

if [[ ! -d $1 || ! -d $2 || ! -r $1 || ! -r $2 ]];then
	echo "First two arguments should be readable directories"
	exit 1
fi

dst_files_cnt=$(find $2 -type f 2>/dev/null | wc -l)
 
if [ $dst_files_cnt -ne 0 ]; then
	echo "DST directory is not empty"
	exit 1
fi

files=$(mktemp)
find $1 -type f -name "*$3*" 2>/dev/null > $files

while read file; do
	new_relative_path=$(echo $file | sed --expression "s/${1}\///g")
	mkdir -p "$2/$(dirname "${new_relative_path}")"
	cp $file $2/$(dirname "${new_relative_path}")
done < $files
