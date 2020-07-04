#!/bin/bash
echo -n 'First way result: '
grep '[0248]' philip-j-fry.txt | grep -vc '[a-w]'
echo -n "Second way result: "
awk '$0~/[2468]/ && $0!~/[a-w]/ {print}' philip-j-fry.txt | wc -l
