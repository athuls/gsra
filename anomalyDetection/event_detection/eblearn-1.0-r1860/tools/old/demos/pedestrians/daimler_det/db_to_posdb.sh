#!/bin/sh

db=$1
posdb="pos_${db}"
echo "Converting db file ${db} to positive only images db ${posdb}..."

# initiate db file
echo -n "" > $posdb

# read bbox file
exec < $db
i=0
while read line
do
    if [ "$line" != ";" ]
    then
	echo $line >> $posdb
    else
	read line
	image=$line
	read line
	image_width=`echo $line | cut -d' ' -f 1`
	image_height=`echo $line | cut -d' ' -f 2`
	read line
	nobjects=`echo $line | cut -d' ' -f 2`
	if [ $nobjects -ne 0 ]
	then
	    # put info back
	    i=`expr $i + 1`
	    echo "keeping $image ($nobjects objects), total positive frames: $i"
	    echo ";" >> $posdb
	    echo "$image" >> $posdb
	    echo "$image_width $image_height" >> $posdb
	    echo "0 $nobjects" >> $posdb
	fi
    fi
done
