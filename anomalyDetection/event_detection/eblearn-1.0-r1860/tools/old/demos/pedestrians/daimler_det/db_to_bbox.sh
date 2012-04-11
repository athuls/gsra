#!/bin/sh

db=$1
bbox="${db}_bbox.txt"
echo "Converting db file ${db} to bbox format ${bbox}..."

# initiate db file
echo -n "" > $bbox

# read bbox file
exec < $db
i=0
while read line
do
    if [ "$line" = ";" ]
    then
	read line
	image=$line
	read line
	read line
	read line
    fi
    if [ `echo $line |cut -c1` = '#' ]
    then
	read line
	read line
	confidence=$line
	class=1
	read line
	w0=`echo $line | cut -d' ' -f 1`
	h0=`echo $line | cut -d' ' -f 2`
	w1=`echo $line | cut -d' ' -f 3`
	h1=`echo $line | cut -d' ' -f 4`
	echo "$image $class $confidence $w0 $h0 $w1 $h1" >> $bbox
    fi
done
