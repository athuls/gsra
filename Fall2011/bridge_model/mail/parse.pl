#!/usr/bin/perl

open FH, "<", "./final_merge.txt";
$count=0;
while($lineread=<FH>){
	$count++;
	@temp=split(/\s+/, $lineread);
	if($temp[$#temp-1] >= 19){
		print "$count\n";
	}
}
