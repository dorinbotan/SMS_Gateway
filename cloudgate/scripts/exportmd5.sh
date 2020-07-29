#!/bin/bash 

### basic variables
DESCPATH=`echo $@ | awk -F" " '{print $1}'`
IMGPATH=`echo $@ | awk -F" " '{print $2}'`
TMPPATH=`echo $@ | awk -F" " '{print $3}'`

### md5sum calculation
MDSUM=`md5sum $IMGPATH | awk -F"  " '{print $1}'`

### commands executed
cat $DESCPATH > $TMPPATH/description.cfg
sed -i "s/(md5sum)/$MDSUM/" $TMPPATH/description.cfg
