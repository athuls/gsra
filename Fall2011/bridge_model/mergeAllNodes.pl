#!/usr/bin/perl -w

open FH3,">","./merge_sparse_nodes_62.txt";

#@files=read_dir('/home/athuls89/Desktop/gsra/bridge_model');
@files=<./sparse_model_62/*>;
#while($count<=17){
foreach $file(@files){
	open FH, "<", "$file";
	while($lineread = <FH>){
		print FH3 $lineread;
	}
	close FH;
}
close FH3;
