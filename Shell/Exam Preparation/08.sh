#!/bin/bash

find -printf "%n %p \n" | sort -rn | head  -5 | cut -d' ' -f2 | rev | cut -d'/' -f1 | rev

