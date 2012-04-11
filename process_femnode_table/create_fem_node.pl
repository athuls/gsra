#!/usr/bin/perl

open FH3,"<","./class_femnode_points";
open FHOUT, ">", "./final_points";
my @final, $count=0;
while($lineread=<FH3>){
	$count++;
	@arr=split(/\s+/,$lineread);
	if($#arr!=7){
		print $#arr, " $lineread      $count ","\n";
	}
	splice @arr,0, 1;
	splice @arr,1,1;
	splice @arr,2,1;
	splice @arr,3,1;
	for($i=0;$i<=$#arr;$i++){
		print $arr[$i],"\t";
	}
	print "\t";
	for($i=0;$i<=$#arr;$i++){
		if($arr[$i]=='0' && $i!=0 && $count<148){
			if($arr[$i]!='|'){
				print $i , " $count ";
			}
		}
	}
	print "\n";
	print FHOUT ";\n";
}
