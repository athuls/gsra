#!/usr/bin/perl

open FH, ">signalTimeValues.txt";

for($i=0;$i<100;$i++)
{
	$b=$i+1;
	$sum=(5*$i)+$b;
	print FH "$i\t$b\t$sum\n";
}

close FH;
