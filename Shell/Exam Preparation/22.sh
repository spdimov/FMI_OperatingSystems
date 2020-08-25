#!/bin/bash

 [ $# -lt 1 ] && echo "Invalid arguments" && exit 1

 [[ ! -r $1 || ! -d $1 ]] && echo "Argument should be readable directory" && exit 2

if  [ $# -eq 2 ]; then
	dest=$2
 elif [ $# -eq 1 ]; then
	 dest="&1"
fi

symlinks=$(mktemp)
find $1 -type l 2>/dev/null -exec file {} \; > $symlinks
cat $symlinks
cnt=0
while read link;do
	if grep -q "broken symbolic link to" <(echo "${link}");then
		cnt=$(($cnt+1))
	else
		stat -c "%N" $(echo $link | cut -d':' -f1 ) 
	fi
done < $symlinks

echo "broken links: $cnt"
