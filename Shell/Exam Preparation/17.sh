#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid arguments"
	exit 1
fi

foo_processes=$(ps -u $1 -o pid='' | wc -l)

users=$(mktemp)

ps -e -o user="" | sort | uniq > users

while read user; do
	current_processes=$(ps -u $user | wc -l)
	if [[ $current_processes -gt $foo_processes && $user != $1 ]]; then
		echo $user
	fi
done < users


avg_time=$(ps -e -o time="" | awk -F':' 'BEGIN {avg=0} {avg+=($1*3600)+($2*60) + $3} END{ avg=avg/NR; printf"%02d:%02d:%02d\n",(avg/3600),((avg/60)%60),(a%60)}')
echo $avg_time

ps -e -o pid="",time="" -u $1 | awk -v avg_time=$avg_time '{ if ($2>avg_time) print $1}'
