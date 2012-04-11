#!/usr/bin/perl -w

open FH3,">","./final_merge.txt";

#@files=read_dir('/home/athuls89/Desktop/gsra/bridge_model');
@files=<*>;
#while($count<=17){
foreach $file(@files){
	open FH, "<", "$file";
	while($lineread = <FH>){
		print FH3 $lineread;
	}
#	print $file."\n";
	close FH;
}

close FH3;
