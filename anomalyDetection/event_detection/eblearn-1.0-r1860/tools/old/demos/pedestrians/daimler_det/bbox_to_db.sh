#!/bin/sh

bbox=$1
db=$bbox.db
tmp=processed.tmp # already processed list
tmp2=image.tmp # all bbox for 1 image
image_width=640
image_height=480
echo "Converting bbox file $1 to daimler db format..."

# initiate db file
echo ":\nSequence_0\n/PATH/\n-1" > $db
echo "" > $tmp

# read bbox file
exec < $bbox
i=0
while read line
do
    i=`expr $i + 1`
    image=`echo $line | cut -d' ' -f 1`
    image=`echo $image | sed s/pgm_.*/pgm/g`
    
    # test if we already processed this image
    grep $image $tmp > /dev/null
    if [ $? -eq 0 ]
    then
	continue # do not process this frame again
    fi
    
    # find all occurences of this image
    cat $bbox | grep $image > $tmp2
    nobjects=`cat $tmp2 | wc -l`
    echo "$i: image=$image nobjects=${nobjects}"

   # output into db
    echo ";" >> $db
    echo "$image" >> $db
    echo "${image_width} ${image_height}" >> $db
    echo "0 ${nobjects}" >> $db
    
    # loop over objects
    for j in `seq 1 $nobjects`
    do
	bb=`head -n $j $tmp2 | tail -n 1`
        
        # extract info from bbox
	class=`echo $bb | cut -d' ' -f 2`
	confidence=`echo $bb | cut -d' ' -f 3`
	w0=`echo $bb | cut -d' ' -f 4`
	h0=`echo $bb | cut -d' ' -f 5`
	w1=`echo $bb | cut -d' ' -f 6`
	h1=`echo $bb | cut -d' ' -f 7`
        # echo "$i: $bb"
	echo -n "   class=$class confidence=$confidence "
	echo "w0=$w0 h0=$h0 w1=$w1 h1=$h1"

	objclass=0 # 0: fully-visible pedestrian
	obj_id=-1
	unique_id=-1

	# output into db
	echo "# ${objclass}" >> $db
	echo "${obj_id} ${unique_id}" >> $db
	echo "$confidence" >> $db
	echo "${w0} ${h0} ${w1} ${h1}" >> $db
	echo "0" >> $db
    done

    # add this image name to a temp file so that we know we processed it
    echo "${image} " >> $tmp
done
