#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid arguments count"
	exit 1
fi

if [[ -d $1 && -r $1 ]] ; then
	find /home/vinicaboy -type l -printf "%Y %p \n" | grep  '^N' | cut -d' ' -f2

fi
