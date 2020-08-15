#!/bin/bash

while read USER HOMEDIR; do
	if [ $(sudo -u $user touch -c $HOMEDIR 2>&1 | grep -q 'Permission denied') ] || [ ! -d $HOMEDIR ]; then
		cat /etc/passwd | awk -v user=$user' FS=":" {if($1==user) print $0}'
	fi
done < <(cat /etc/passwd | cut -d':' -f16)
