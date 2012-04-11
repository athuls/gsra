#!/usr/bin/perl

@files=</home/athuls89/Desktop/gsra/Winter2012/Pictures_Ettouney/June+25+-+2010+-+2/*>;
$count=0;

foreach $file(@files)
{
	system("mv $file $count.JPG");
	$count++;
}
