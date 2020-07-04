#!/bin/bash 

find /home -maxdepth 1 -newerct 100 -not -newerct '@1000000000000' -exec grep :{}: /etc/passwd \; | cut -d':' -f1,5 | cut -c2- | cut -d',' -f1 | tr ':' '\t'
