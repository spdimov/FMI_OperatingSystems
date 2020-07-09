#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Invalid arguments"
	exit 1
fi

text_file=$1
string_one=$2
string_two=$3

if [[ ! -f "$text_file" || ! -r "$text_file" ]];then
       	echo "First argument is invalid"
	exit 1

fi

terms_one=$(grep  "^$string_one=" $text_file | cut -d'=' -f2 | tr -s ' '| tr ' ' '\n' | sort | uniq)
terms_two=$(grep  "^$string_two=" $text_file | cut -d'=' -f2 | tr -s ' '| tr ' ' '\n' | sort | uniq)


common_terms=$(comm -12 <( (echo "$terms_one") ) <( (echo "$terms_two") ) )

for term in $common_terms;do
	terms_two=$( echo "$terms_two" | sed "s/$term//g" | tr '\n' ' ' | cut -c2- )
done

echo "$terms_two"

sed -i -e "s/^${string_two}=.*/${string_two}=${terms_two}/g" $text_file 



 
