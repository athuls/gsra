#!/bin/sh

i=0
while [ $i != 128 ]
do
	mkdir "position$i"
	i=`expr $i + 1`	
done
