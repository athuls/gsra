#!/usr/bin/perl

open FH, "<", "./below_deck";
$count=0;
while($lineread=<FH>){
	$count++;
	@temp=split(/\s+/, $lineread);
	if($temp[$#temp] ==$ARGV[0]){
		print "$count\n";
	}
}
