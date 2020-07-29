#!/bin/bash

if [ $# -ne 2 ] && [ $# -ne 3 ]; then
	echo "********************************************************"
	echo "* script to flash the cloudgate via the upgrade daemon *"
	echo "********************************************************"
	echo "usage: $0 <host (like you use with ssh)> <file> [<port_for_upgraded>]"
	echo "example: $0 cg myfile.zip"
	echo "example: $0 cg - < myfile.zip"
	exit -1
fi

if [ "$2" != "-" ]; then
	# we always stream the file over ssh so recall this script using stream mode
	exec $0 $1 - $3 < $2
fi

PORT_UPG=${3:-$((${RANDOM%6} + 9230 ))}

rmt_cmd=\
"[ \"\$(id -u)\" -ne \"0\" ] && { ttmv -c provd -r launch_upgraded -t int -a $PORT_UPG; } || { killall upgraded; /usr/sbin/upgraded -p $PORT_UPG; };"\
"curl -# -F \"input=@-;type=multipart/form-data\" localhost:$PORT_UPG";

ssh $1 "$rmt_cmd"
