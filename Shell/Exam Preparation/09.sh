#!/bin/bash

find -type f -printf "%T@ %p %i \n" | sort -rn -t' ' -k1 | cut -d' ' -f3 | head -1

