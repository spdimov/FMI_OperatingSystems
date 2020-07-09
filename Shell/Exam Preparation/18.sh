#!/bin/bash

users_home=$(mktemp)

cat /etc/passwd | cut -d':' -f1,6 | tr ':' ' ' > users_home


most_recently_changed=$(mktemp)
while read user home; do
	if [[ ! -d $home ]];then 
		exit 1
	fi

	find $home -type f 2>/dev/null -printf "%T@ %f %u\n" | sort -n | tail -1 >> most_recently_changed
done < users_home


cat most_recently_changed | sort -n | tail -1 | cut -d':' -f3
