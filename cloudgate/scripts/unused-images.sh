#!/bin/bash

###
# bash script to loop into the image files and check for unused image in the sourcecode 
###

COUNTER=0; 
for myImage in `find ../package/webgui/files/www/static/images -type f -exec basename {} \;`
do
	if [ `grep -r "$myImage" ../package/webgui/files/www | wc -l ` -eq 0 ]
	then
		find ../package/webgui/files/www/static/images -name "$myImage" -exec git rm {} \; 		
		let COUNTER=COUNTER+1 
	fi 
done
echo "there are $COUNTER unused images"
