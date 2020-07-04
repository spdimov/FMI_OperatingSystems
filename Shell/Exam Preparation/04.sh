#!/bin/bash

if [ $# -ne 2 ]; then
	exit 1;
fi

fileOneResult=$(grep "${1}" "${1}" | wc -l)
fileTwoResult=$(grep "${2}" "${2}" | wc -l)

if [ $fileOneResult -gt $fileTwoResult ]; then
	cut -d' ' -f4- "${1}" | sort -t' ' -k1 > $1.songs
else cut -d' ' -f4- "${2}" | sort -t' ' -k1
fi

