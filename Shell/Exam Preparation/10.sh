#!/bin/bash

sed -n '1,$p' 10.txt | awk 'BEGIN {FS="[:,]"} {if($0~/\/SI\//)print $1 " " $5 " "$(NF-1)}' | sort -n -t' ' -k1 | cut -d' ' -f2-
