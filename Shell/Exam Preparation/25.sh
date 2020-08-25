#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid arguments"
	exit 1
fi
LOGDIR=$1
friends=$(mktemp)
find $LOGDIR -maxdepth 3 -mindepth 3 -type d | cut -d'/' -f4 | sort | uniq > $friends
friends_lines=$(mktemp)
while read friend;do
	echo "$friend $(find $LOGDIR -maxdepth 4 -mindepth 4 -type f -regex ".*/$friend/.*" -exec wc -l {} \; | awk 'BEGIN{sum=0} {sum+=$1} END{print sum}')" >> $friends_lines

done < $friends

sort -rn -t ' ' -k2 $friends_lines | cut -d' ' -f1
