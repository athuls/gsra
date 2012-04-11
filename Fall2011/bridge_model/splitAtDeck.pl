#!/usr/bin/perl

open FileMerged, "<", "./mail/final_merge.txt";
open FileVisibility, "<", "./elementVisibility";

open FileAboveDeck,">","./above_deck";
open FileBelowDeck,">","./below_deck";

my $count=1, @filehandle;

my %aboveDeck;
my %belowDeck;

while($lineread=<FileVisibility>){
	@arr=split(/\s+/,$lineread);		
	if($arr[1]==0){
		print "I am above deck\n";
		$aboveDeck{$arr[0]}=1;
	}
	elsif($arr[1]==1){
		print "I am below deck\n";
		$belowDeck{$arr[0]}=1;
	}
		
}

$sizeAboveDeck=keys %aboveDeck;
$sizeBelowDeck=keys %belowDeck;

print "$sizeAboveDeck and $sizeBelowDeck\n";

$countAboveDeck=0;
$countBelowDeck=0;

while($lineread = <FileMerged>){
	@arr1=split(/\s+/, $lineread);
	if($aboveDeck{$arr1[$#arr1]}==1){
		$countAboveDeck++;
		print FileAboveDeck "$lineread";
	}
	elsif($belowDeck{$arr1[$#arr1]}==1){
		$countBelowDeck++;
		print FileBelowDeck "$lineread";
	}
}

print "$countAboveDeck and $countBelowDeck\n";

close FileMerged;
close FileVisibility;
close FileAboveDeck;
close FileBelowDeck;

