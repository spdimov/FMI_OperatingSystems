#!/bin/bash

users=$(ps -e -o user="" | sort | uniq)

while read user; do
	ps -u "${user}" -e -o rss | awk -v user=$user '{cnt=cnt+$1} END{print user" "cnt}'
done < <(echo $users | tr ' ' '\n')

while read user; do
	max_rss=$(ps -u "${user}" -e -o rss="",pid="" | sort -rn | head -1)
	ps -u "${user}" -e -o rss="",pid="" | awk -v user=$user -v max_rss=$(echo $max_rss | cut -d' ' -f1) -v pid=$(echo $max_rss | cut -d' ' -f2) '{cnt=cnt+$1} END { if(cnt/NR*2<max_rss) print pid }'
done < <(echo $users | tr ' ' '\n')

