#!/bin/bash

find -maxdepth 2 -size 0 -exec rm -i {} \;

find ~ -maxdepth 2 2>/dev/null -printf "%s %p \n" | sort -n -t' ' -k1 | cut -d' ' -f2- | tail -5 | xargs rm -i
