#!/bin/sh

# head_over_ped=.3 # ratio of head/ped heights
# head_center=.4 # position of center in bbox's height starting from top
# trunk_over_ped=.6 # ratio of trunk/ped heights
# trunk_center=.2 # position of center in bbox's height starting from top
# woverh=.5 # ratio of width/height for head and trunk classes
#inria=$1
inria=/data/pedestrians/inria/INRIAPerson
#inria=${HOME}/${HOSTNAME}adata/ped/inria/INRIAPerson
annotations=$inria/Train/annotations
annotations2=$annotations/../pascal_annotations
annotations=${HOME}/visiongrader/datasets/pedestrians/inria/ignore/Train/
annotations2=${HOME}/visiongrader/datasets/pedestrians/inria/ignore/Train/pos/
echo "Converting INRIA annotations ${annotations} to PASCAL format ${bbox}..."
ECHO="echo"
#ECHO="echo -e"

mkdir -p $annotations2

# read bbox file
max_himage_over_hped=0.0
first_time=1
max_area=0.0
min_area=0.0
cd $annotations
for i in `ls *.txt`
do
    # initiate db file
    bbfile=$annotations2/$i.xml
    ${ECHO} "<annotation>" > $bbfile
    exec < $i
    while read line
    do
	if [ "$line" = "" ]
	then
	    continue
	fi
	if [ `${ECHO} $line |cut -c1` = '#' ]
	then
	    continue
	fi
	tmp=`expr substr "${line}" 1 17`
	if [ "$tmp" = "Image filename : " ]
	then
	    len=`expr length "${line}"`
	    len=`expr $len - 19`
	    fname=`expr substr "${line}" 19 ${len}`
	    fname=`basename ${fname}`
	    ${ECHO} "\t<filename>${fname}</filename>" >> $bbfile
	fi
	tmp=`expr substr "${line}" 1 25`
	if [ "$tmp" = "Image size (X x Y x C) : " ]
	then
	    len=`expr length "${line}"`
	    len=`expr $len - 26`
	    imsize=`expr substr "${line}" 26 ${len}`
	    hsize=`${ECHO} $imsize | cut -d' ' -f1`
	    wsize=`${ECHO} $imsize | cut -d' ' -f3`
	fi
	tmp=`expr substr "${line}" 1 22`
	if [ "$tmp" = "Center point on object" ]
	then
	    len=`expr length "${line}"`
	    len=`expr $len - 48`
	    center=`expr substr "${line}" 48 ${len}`
	    center=`${ECHO} $center | sed -e 's/[,)(-]//g'`
	    wcenter=`${ECHO} $center | cut -d' ' -f1`
	    hcenter=`${ECHO} $center | cut -d' ' -f2`
	fi
	tmp=`expr substr "${line}" 1 23`
	if [ "$tmp" = "Bounding box for object" ]
	then
	    len=`expr length "${line}"`
	    len=`expr $len - 70`
	    bb=`expr substr "${line}" 70 ${len}`
	    bb=`${ECHO} $bb | sed -e 's/[,)(-]//g'`
	    
	    # add a pedestrian object
	    x1=`${ECHO} $bb | cut -d' ' -f1`
	    y1=`${ECHO} $bb | cut -d' ' -f2`
	    x2=`${ECHO} $bb | cut -d' ' -f3`
	    y2=`${ECHO} $bb | cut -d' ' -f4`
	    ${ECHO} "\t<object>" >> $bbfile
	    ${ECHO} "\t\t<name>ped</name>" >> $bbfile
	    ${ECHO} "\t\t<center>" >> $bbfile
	    ${ECHO} "\t\t\t<x>${wcenter}</x>" >> $bbfile
	    ${ECHO} "\t\t\t<y>${hcenter}</y>" >> $bbfile
	    ${ECHO} "\t\t</center>" >> $bbfile
	    ${ECHO} "\t\t<bndbox>" >> $bbfile
	    ${ECHO} "\t\t\t<xmin>${x1}</xmin>" >> $bbfile
	    ${ECHO} "\t\t\t<ymin>${y1}</ymin>" >> $bbfile
	    ${ECHO} "\t\t\t<xmax>${x2}</xmax>" >> $bbfile
	    ${ECHO} "\t\t\t<ymax>${y2}</ymax>" >> $bbfile
	    ${ECHO} "\t\t</bndbox>" >> $bbfile
	    ${ECHO} "\t</object>" >> $bbfile
	    wped=`expr $x2 - $x1`
	    hped=`expr $y2 - $y1`

# 	    # add a head object
# 	    hhead=`${ECHO} "$head_over_ped * $hped" | bc`
# 	    whead=`${ECHO} "$woverh * $hhead" | bc`
# 	    hx1=`${ECHO} "$wcenter - $whead * .5" | bc`
# 	    hy1=`${ECHO} "$hcenter - $hhead * $head_center" | bc`
# 	    hx2=`${ECHO} "$hx1 + $whead" | bc`
# 	    hy2=`${ECHO} "$hy1 + $hhead" | bc`
# 	    ${ECHO} "\t<object>" >> $bbfile
# 	    ${ECHO} "\t\t<name>head</name>" >> $bbfile
# 	    ${ECHO} "\t\t<bndbox>" >> $bbfile
# 	    ${ECHO} "\t\t\t<xmin>${hx1}</xmin>" >> $bbfile
# 	    ${ECHO} "\t\t\t<ymin>${hy1}</ymin>" >> $bbfile
# 	    ${ECHO} "\t\t\t<xmax>${hx2}</xmax>" >> $bbfile
# 	    ${ECHO} "\t\t\t<ymax>${hy2}</ymax>" >> $bbfile
# 	    ${ECHO} "\t\t</bndbox>" >> $bbfile
# 	    ${ECHO} "\t</object>" >> $bbfile

# 	    # add a trunk object
# 	    htrunk=`${ECHO} "$trunk_over_ped * $hped" | bc`
# 	    wtrunk=`${ECHO} "$woverh * $htrunk" | bc`
# 	    tx1=`${ECHO} "$wcenter - $wtrunk * .5" | bc`
# 	    ty1=`${ECHO} "$hcenter - $htrunk * $trunk_center" | bc`
# 	    tx2=`${ECHO} "$tx1 + $wtrunk" | bc`
# 	    ty2=`${ECHO} "$ty1 + $htrunk" | bc`
# 	    ${ECHO} "\t<object>" >> $bbfile
# 	    ${ECHO} "\t\t<name>trunk</name>" >> $bbfile
# 	    ${ECHO} "\t\t<bndbox>" >> $bbfile
# 	    ${ECHO} "\t\t\t<xmin>${tx1}</xmin>" >> $bbfile
# 	    ${ECHO} "\t\t\t<ymin>${ty1}</ymin>" >> $bbfile
# 	    ${ECHO} "\t\t\t<xmax>${tx2}</xmax>" >> $bbfile
# 	    ${ECHO} "\t\t\t<ymax>${ty2}</ymax>" >> $bbfile
# 	    ${ECHO} "\t\t</bndbox>" >> $bbfile
# 	    ${ECHO} "\t</object>" >> $bbfile

# 	    # find min/max bbox over entire set
# 	    if [ $first_time -eq 1 ] ; then
# 		first_time=0
# 		max_area=`${ECHO} "${hped} * ${wped}" | bc`
# 		min_area=`${ECHO} "${hped} * ${wped}" | bc`
# 	    else
# 		area=`${ECHO} "${hped} * ${wped}" | bc`
# 		sup=`${ECHO} "${area} > ${max_area}" | bc`
# 		if [ $sup = 1 ] ; then max_area=$area ; fi
# 		sup=`${ECHO} "${area} < ${min_area}" | bc`
# 		if [ $sup = 1 ] ; then min_area=$area ; fi
# 	    fi
# 	    hh=`${ECHO} "scale=5;$hsize / $hped" | bc`
# 	    sup=`${ECHO} "$hh > $max_himage_over_hped" | bc`
# 	    if [ $sup -eq 1 ] ; then
# 		max_himage_over_hped=$hh
# 	    fi
# 	    ${ECHO} "$fname (${hsize}x${wsize}) $bb $center | ped ${hped}x${wped} | trunk ${htrunk}x${wtrunk} | head ${hhead}x${whead} | maxhh ${max_himage_over_hped} | min_area $min_area max_area $max_area"

	fi
    done
    echo "</annotation>" >> $bbfile
#    cat $bbfile
done
