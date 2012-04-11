#!/usr/bin/perl -w

#open FH3,">","./final_merge.txt";

#my $count=1, @filehandle;

#@files=read_dir('/home/athuls89/Desktop/gsra/bridge_model');
@files=<*>;
#while($count<=17){
foreach $file(@files){
	#open FH, "<", "$count.txt";
	#while($lineread = <FH>){
	#	print FH3 $lineread;
	#}
	print $file."\n";
#	$count++;
}
