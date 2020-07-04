#!/bin/bash

cat 06.txt | grep '\Inf' | cut -d',' -f1 | awk 'BEGIN {FS=":"} { if ($5~/a$/) print $1;}' | cut -c3-4 | sort -n | uniq -c | tail -n 1
