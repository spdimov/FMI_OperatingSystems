#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid arguments"
	exit 1
fi

if ! [[ $1 =~ ^[+-]?[0-9]+$ ]]; then
       	echo "Invalid format of argument"
       	exit 1
fi

USERS=$(mktemp)

ps -e -o user | cut -d' ' -f1 | sort  | uniq > USERS

while read user
do
	to_kill=$(mktemp)
	ps -e -o rss,pid,user | grep  $user | sort -rn | awk -v max=$1 'NR==1 {ppid=$2; cnt=0} {cnt=cnt+$1} END{if(cnt>max) print ppid}' >to_kill
	kill $(cat to_kill)


done < USERS

