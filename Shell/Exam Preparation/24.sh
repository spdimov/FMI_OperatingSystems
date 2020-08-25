#!/bin/bash

check_for_nonexistent_home_dir(){
	home_dir=$1
	if [ -d $home_dir ];then
			
		false
	else
		true
	fi
}
check_if_user_cant_write(){
	home_dir=$1
	if [[ $(stat -c "%A" $1 | cut -c3) =~ "w" ]];then
		false
	else
		true
	fi
}
check_if_user_is_not_owner(){
	home_dir=$1
	user=$2
	if [ $(stat -c "%U" $1) == $user ] ;then
		false
	else
		true
	fi
}
root_rss=$(ps -u "root" -o rss="" | awk '{cnt+=$1}END{print cnt}')

while read user home; do
if check_for_nonexistent_home_dir $home || check_if_user_cant_write $home || check_if_user_is_not_owner $home $user;then
	current_rss_sum=$(ps -e -o uid="",rss="" | grep $user | awk 'BEGIN{cnt=0} {cnt=cnt+$2} END{print cnt}')
	if [ $current_rss_sum -gt $root_rss ]; then
		echo "Deleting user with uid $user processes"
		#pkill -U $user
	fi
fi
done < <(cut -d':' -f3,6 /etc/passwd | tr ':' ' ')
