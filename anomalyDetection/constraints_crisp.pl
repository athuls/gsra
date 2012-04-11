#!/usr/bin/perl

sub modifySymbol
{
	$retVal = '\''.'$'.$_[0].'\'';	
}

sub checkSymbols
{
	$retVal=$symbolHash{$_};			
}

print "Please enter the number of symbols in the constraints equations : ";
$numSymbols=<>;

our %symbolHash;

my @symbol;
print "Please enter names of the symbols in the order in which they appear in the input file : ";
for($i=1;$i<=$numSymbols;$i++)
{
	#Modify symbol to change it from say x to $x and so on
	print "Symbol $i : ";
	$symbol[$i-1]=<>;
	chomp($symbol[$i-1]);
}	

print "Please enter the number of constraints:";

$numConstraints = <>;

my @constraints;
my @symbolValues;
our %varIndexMap;

for($i=1;$i<=$numConstraints;$i++)
{
	print "Enter constraint $i : ";
	$constraints[$i-1]=<>;
	chomp($constraints[$i-1]);
	
	for($j=0;$j<$numSymbols;$j++)
	{
		$tempSymbol=$symbol[$j];
		$tempScalar=&modifySymbol($tempSymbol);
		$tempScalar =~ tr/'//d;
		$varIndexMap{$tempScalar}=$j;
		print "$tempScalar\n";
		$constraints[$i-1] =~ s/$tempSymbol/$tempScalar/ge;
		$constraints[$i-1] =~ tr/'//d;
		$index=$i-1;
		print "$constraints[$index]\n";
	}
	print "Modified constraint:  $constraints[$i-1]\n";
}

print "Enter path to the file with values for all variables:";
$filePath=<>;

open FH, "<$filePath";

#reading in the symbol values from time series data into symbolValues
while(<FH>)
{
	push @symbolValues, [split(/\s+/,$_)];	
} 

#Constraint checking

our @varValues;
our $count=0;

#For every data point
for($i=1;$i<$#symbolValues;$i++)
{
	#For every symbol	
	for($j=0;$j<=$#{$symbolValues[$i]};$j++)
	{
		$tempSymbol=&modifySymbol($symbol[$j]);
		$tempSymbol =~ tr/'//d;	
		$varValues[$j]=$symbolValues[$i][$j];		
	}
	
	for($k=0;$k<$numConstraints;$k++)
	{
		#script to execute in eval
		$tempConstraint=$constraints[$k];
		$tempConstraint  =~ s/(\$\w+)/$varValues[$varIndexMap{$1}]/g;
		print "the new constraint is $tempConstraint\n";
		$evalValue=eval $tempConstraint;
		if($evalValue != 1)
		{
			print "Constraint not satisfied - $tempConstraint\n";	
		}
	}
}

