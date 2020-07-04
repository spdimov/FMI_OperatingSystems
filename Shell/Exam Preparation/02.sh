#!/bin/bash
cat $1 | cut -d" " -f4- | nl -s'. ' | sort -t' ' -k2
awk '{print NR".",$1=$2=$3="",$0}' 02.txt | tr -s ' ' | sort -t' ' -k2
awk '{print NR".",$1=$2=$3="",$0}' $1 | awk '{$1=$1}1' | sort -t' ' -k2


