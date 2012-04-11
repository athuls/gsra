#!/bin/sh

full=/home/sermanet/matlab/caltech_pedestrians/data-USA/usa-train_full_test/
ann=/home/sermanet/matlab/caltech_pedestrians/data-USA/usa-train_annotations/
pos=/home/sermanet/matlab/caltech_pedestrians/data-USA/usa-train_full_test_pos/

cd $full
for i in `find * -name "*.jpg"`; 
do 
  f=`echo $i | cut -d'.' -f1`; 
  d=`dirname $f`
  a="${ann}${f}.txt"
  test -e $a
  if [ $? -eq 1 ] 
      then
      echo "$i $a exists";
      mkdir -p $pos/$d
      ln -s $full/$i $pos$f.jpg
  fi
done