#!/bin/bash

find /home/vinicaboy -inum $(find ~vinicaboy -type f -printf "%T@ %i %p \n" | sort -n | tail -n1 | cut -d' ' -f2) | grep -o '.' | grep '/' | wc -l
