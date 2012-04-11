#!/bin/sh

i=0
while [ $i != 508 ]
do
	mkdir "position$i"
	i=`expr $i + 1`	
done
