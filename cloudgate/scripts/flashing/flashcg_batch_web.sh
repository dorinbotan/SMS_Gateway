#!/bin/sh

#
# This script can be used to flash a batch of images to the cloudgate
# It is smart enough to see that if e.g. you put a gobi image in your batch,
# and your current FW is to old to flash this type of image, it will first
# upgrade the FW, reboots and then flash the rest
#
# to create a batch you need to make a dir which holds all the .zip files to flash.
# They will be flash in alphabetical order so prepend the filename with 1_ to adjust the order.
# (kernel/rootfs should be flashed first)
# 
# the zip files need te be flashable via the webgui, meaning
# - files from the variant DB need to be rezipped
# - gobi files need to be splitted (script available)
#
# In the dir you also need to create a file named 'models'.
# In this file you need to specify on which models this 'pack' may be flashed.
# Tt can be any combination of 'gobi-hpex-telit-nomodem'
# The typical use case for this is that if the pack contains a modem FW, only
# that model is allowed, if not than all models will be OK
#
# you can also let this script select the modem family you want by
# making a file named 'activefamily' in the batchdir which holds the
# name of the selected image as displayed on the webgui (take note this
# does not have to be the same as the actual family, but the webgui
# only knows the display name)
#
# after creating the batch just call this script with param batchdir='/path/to/my/batch/dir'
#
# alternative you can zip the contents of that batch dir and then
# call the script with param file='/path/to/my/zipped_batch.zip'
# take note that this will slow down the process since this big
# zip is unzipped each time this script is invoked.
#
#
# HISTORY
# 0.1 - first hand out
# 0.2 - initial version
# 0.3 - add support for .bin
#     - reboot at the end
# 0.4 - stream file to curl
#     - fatal on file upload failure
# 0.5 - add support to select a modem family
#     - add progress indication
#     - correction for unsupported params
#     - fixed reflashing fw on batch re-enter
#     - added support for the new proxy upload approach
# 0.6 - added support for special chars in the name
#     - select images after reboot to support name changes
# 0.7 - use gawk iso awk for cygwin compatibility

#consts
readonly version="0.7"
readonly argsSupported=" file ip user pwd debug batchdir skipreboot "

#user vars defaults
ip=192.168.1.1
user=admin
pwd=admin
debug=0
skipreboot=0
file=""
batchdir=""

usage()
{
	echo "*****************************************************************"
	echo "*"
	echo "* $(basename $0) version $version"
	echo "*"
	echo "* usage: $(basename $0) key1=value1 [key2=value2] ..."
	echo "*"
	echo "* supported keys:"
	for arg in $argsSupported
	do
		echo "*   - $arg"
	done
	echo "*"
	echo "* example"
	echo "*    $(basename $0) file=myCombiFile1.zip"
	echo "*"
	echo "*****************************************************************"
}


construct_subvars()
{
	url=http://$ip
	tmpdir=$(mktemp -d)
	debug "tmpdir=$tmpdir"
	readonly cj=$tmpdir/mycookyjar
	readonly cookies="-b $cj -c $cj"
	readonly curl="curl -s -b $cj -c $cj --connect-timeout 5"
}

debug()
{
	[ "$debug" != "0" ] && echo "	DEBUG" "$1"
	return 0
}

error()
{
	stop_progress_indication
	echo "ERROR:" "$1" >&2
}

fatal()
{
	error "$1" >&2
	exit 1
}

json_get() 
{
	#this is not correct, but correct enough for our usecase
	out=$(echo $1 | grep -soE "\"$2\"[ ]*:[^,}]+" | grep -soE ":.*" | grep -soE "[^:]*")
	out=${out//\"}
	echo $out
}

cg_web_reset()
{
	$curl -data '' $url/api/reset
}

cg_web_auth()
{
	$curl -j -X POST -H "Content-Type: application/json" -d '{"username":"'$user'","password":"'$pwd'"}' $url/api/login | grep -q AUTHENTICATION_SUCCESS
}

cg_web_modem_info()
{
	local minf

	for i in 0 1 2 3 4 5 6 7 8 9
	do
		minf=$($curl $url/api/modem)
		[ "$minf" != "" ] && break
		sleep 5
	done

	echo "$minf"
}

cg_web_info()
{
	local sys
	local prov
	local modem
	local cnt=0

	echo -n "reading info: "

	sys=$($curl $url/api/system_info)
	prov=$($curl $url/api/provisioning)

	serial=$(json_get "$sys" "serial_nbr")
	ver_fw=$(json_get "$prov" "fwVersion")
	ver_conf=$(json_get "$prov" "configVersion")
	ver_img=$(json_get "$prov" "imageVersion")

	if [ "$serial" == "" ] || [ "$ver_fw" == "" ]; then
		fatal "cannot get the device info"
	fi

	while true
	do
		modem=$($curl $url/api/modem)
		model=$(json_get "$modem" "modem_model")
		#bail out if model was found or we know the serial is from a 'no-modem' model
		[ "$model" != "" ] && break
		echo $serial | grep -iqE "MZ|MH|MX" && break
		[ "$cnt" == "15" ] && debug "no model found, assume no-modem" && break
		sleep 4 && echo -n '*' && cnt=$((cnt+1))
	done

	model=${model:-"nomodem"}
	echo "OK"
}

cg_print_info()
{
	echo "CURRENT INFO:"
	echo "  MODEM:  * $model"
	echo "  SERIAL: * $serial"
	echo "  FW:     * $ver_fw"
	echo "  IMG:    * $ver_img"
	echo "  CONF:   * $ver_conf"
}

cg_web_upload()
{
	local rsp
	local port

	#we have seen uploads to fail now and then, retry 3 times
	for cnt in 1 2 3
	do
		[ "$cnt" != "1" ] && debug "sleeping 10 seconds before retry" && sleep 10
		rsp=$($curl $url/api/provisioning_port)
		port=$(json_get "$rsp" port)
		method=$(json_get "$rsp" method)
		if [ "$method" == "proxy" ]; then purl="$url/upload"; else purl="$url:$port"; fi;
		debug "cg_web_upload: port=$port method=$method file=$1 cnt=$cnt purl=$purl"
		# stream directly to curl. because of a bug in upgraded this has more chance on success ...
		cat "$1" | curl -v -b $cj -c $cj -H "EXPECT:" -F "input=@-;type=multipart/form-data" $purl 2>&1 | grep -q 'HTTP/1.1 200 OK' && return 0
	done

	return 1
}

cg_version_gt()
{
	local v1
	local v2
	v1=$(echo "$1" | grep -oE "[0-9]+\.[0-9]+\.[0-9]+" | sed 's/\./ /g' | gawk '{printf ($1*1000000)+($2*1000)+$3}')
	v2=$(echo "$2" | grep -oE "[0-9]+\.[0-9]+\.[0-9]+" | sed 's/\./ /g' | gawk '{printf ($1*1000000)+($2*1000)+$3}')
	v1=${v1:-0}
	v2=${v2:-0}
	debug "v1: $v1 <- $1"
	debug "v2: $v2 <- $2"
	if [ "$v1" -gt "$v2" ];
	then
		return 0
	else
		return 1
	fi
}

cg_upload_check()
{
	local cur_ver
	cur_ver=$2
	cts=$(unzip -p "$1" bundle.cfg | grep -ioE "m2mbox.*=")
	cts=${cts,,} #make lower case just te be sure
	[ "$cts" == "" ] && fatal "did not find any content type in file $file"
	for ct in $cts
	do
		case $ct in
			m2mbox_kernel*);;
			m2mbox_root*);;
			m2mbox_prov*);;
			m2mbox_conf*);;
			m2mbox_sconf*);;
			m2mboxgobi*)
				cg_version_gt "$cur_ver" "1.22.0" || return 1
				;;
			m2mbox_ln930*)
				cg_version_gt "$cur_ver" "1.27.0" || return 1
			;;
			*)
			fatal "not supporting type $ct"
		esac
	done

	return 0
}


batch_process()
{
	#get the allowed models for this package
	models_allowed=$(cat $batchdir/models)
	if [ "$models_allowed" == "" ]; then
		fatal "no allowed models specification found"
	fi 

	#do login
	echo -n "logging in to cg ($ip):"
	while true
	do
		cg_web_auth && echo OK && break
		echo -n '*'
	done

	# get current info
	cg_web_info
	cg_print_info

	# check against the allowed model
	echo "$models_allowed" | grep -q "$model" || fatal "package is for $models_allowed but found $model"

	# now upload all files
	for file in $(find $batchdir -maxdepth 1 | grep -iE '\.(bin|zip)$' | sort)
	do
		debug "didreboot=$didreboot file=$file filesdone=$filesdone"

		# check if file was already done
		echo -n $filesdone | grep -q "$(basename "$file")" && debug "skipping $file (already done)" && continue

		#check if we can flash the given file with the current fw version
		(echo "$file" | grep -iqE 'bin$') || cg_upload_check "$file" "$ver_fw"
		if [ $? -ne 0 ] 
		then
			[ "$didreboot" == "1" ] && fatal "fw outdated (can not handle $file)"
			#fw is to old, lets reboot and hope we already flashed the needed fw :)
			echo "current FW is to old to flash $file, need to reboot before proceeding"
			echo "rebooting ..."
			cg_web_reset
			didreboot=1
			echo -n "sleeping 150 seconds:"; start_progress_indication; sleep 150; stop_progress_indication; echo " OK"
			#lets try it again
			batch_process
			return
		fi

		echo -n "flashing $file: "
		start_progress_indication
		cg_web_upload "$file" || fatal "failed to flash file !"
		stop_progress_indication
		echo " OK"
		filesdone="$filesdone;;; $(basename "$file")"
	done

}



argsExp()
{
	for kv in "${@:2}"
	do
		key=$(echo $kv | grep -oE '^[^=]+')
		val=$(echo $kv | grep -oE '=.*$' | sed 's/=//')
		[ "$key" == "" ] && fatal "invalid argument \"$kv\""
		echo "$1" | grep -q " $key " || fatal "unsupported param \"$key\""
		eval "$key='$val'"
	done

	for var in $1; do debug "args: $var=$(eval echo "\$$var")"; done
}

handle_batch_file()
{
	unzip -q "$1" -d "$tmpdir" || fatal "cannot extract $1"
	batchdir="$tmpdir"
}

cleanup()
{
	stop_progress_indication

	if [ "$debug" -lt 2 ];
	then
		[ "$tmpdir" != "" ] && debug "cleanup $tmpdir" && rm -rf "$tmpdir"
	else
		debug "skipping cleanup of $tmpdir"
	fi
}

select_modem_family()
{
	local mod
	local fami
	local id
	local istat
	local selimg

	debug "get image id for family \"$1\""

	mod=$(cg_web_modem_info)
	[ "$mod" == "" ] && fatal "could not get modem info"
	esc=$(echo $1 | sed 's/[][\.|$(){}?+*^]/\\&/g')
	fami=$(echo $mod | grep -oiE '{[^{}]*"'"$esc"'"[^{}]*}')
	
	[ "$fami" == "" ] && fatal "family does not exists"
	id=$(json_get "$fami" "id")
	[ "$id" == "" ] && fatal "no id found for family"

	debug "activate family"
	mod=$(echo $mod | sed 's/"selectedImageId": "[^"]*"/"selectedImageId": "'"$id"'"/g' )
	$curl -X POST -H "Content-Type: application/json" -d "$mod" $url/api/modem 1>/dev/null 2>&1

	debug "verify family"
	mod=$(cg_web_modem_info)
	selimg=$(json_get "$mod" "selectedImageId")

	[ "$id" != "$selimg" ] && fatal "failed to activate family"
}

main()
{
	trap cleanup EXIT
	[ $# -eq 0 ] && usage
	argsExp "$argsSupported" "$@"
	construct_subvars
	[ "$file" != "" ] && handle_batch_file "$file"
	[ "$batchdir" == "" ] && fatal "no directory specified where to find the batch to flash"
	batch_process

	if [ "$skipreboot" != "0"  ]
	then
		echo "upgrading done, skipping reboot but take note that image selection might fail"
	else
		cg_web_reset
		echo "upgrading done, rebooting ... "
		echo -n "sleeping 150 seconds:"; start_progress_indication; sleep 150; stop_progress_indication; echo "OK"
		cg_web_auth
		cg_web_info
		cg_print_info
	fi

	#if needed select a new modem image (need to be at least 2 chars)
	active_family=$( [ -e $batchdir/activefamily ] && cat $batchdir/activefamily | grep -oE '\S+.*\S+' -m 1 )
	if [ "$active_family" != "" ]
	then
		echo -n "trying to activate family \"$active_family\": "
		start_progress_indication
		select_modem_family "$active_family"
		stop_progress_indication
		echo " OK"
	fi

	echo "**********"
	echo "*  DONE  *"
	echo "**********"
	exit 0
}


start_progress_indication()
{
	if [ "$pi_pid" == "" ]
	then
		( while true; do echo -n "*"; sleep 4; done ) &
		pi_pid="$!"
	fi
}

stop_progress_indication()
{
	# special trick to mute the terminate message, you don't need to mute the kill cmd itself, but redirect the 'wait' output
	[ "$pi_pid" != "" ] && kill "$pi_pid" && wait "$pi_pid" 1>/dev/null 2>&1
	pi_pid=""
}

main "$@"
 





