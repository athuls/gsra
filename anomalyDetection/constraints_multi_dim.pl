#!/usr/bin/perl
use warnings NONFATAL => 'all', FATAL => 'uninitialized';
#use strict;
#no autovivification qw(strict fetch exists delete);

sub modifySymbol
{
	$retVal = '\''.'$'.$_[0].'\'';	
}

#hash reference not being updated
sub recurse
{
	my $hashRef=$_[0];
	$count=0;
	#$size=keys(%$hashRef);
	foreach my $keyDim(reverse sort keys %$hashRef)
	{
		if(ref($hashRef->{$keyDim})){
			&recurse($hashRef->{$keyDim});		
		}
		else{
#			print "$keyDim and $hashRef->{$keyDim}\n";
		}
		$count++;
	}
}	

#infer the value of a signal, given all the dimension values
sub infer_value
{
	my $signal = $_[0];
			
}

sub transform
{
	#work on original copy of constraints
	for(my $i=0;$i<=$#_;$i++){
		foreach my $signalTemp(@symbol){
			$_[$i] =~ s/$signalTemp/signalRef->{$&}/g;
		}
	}
}

our @expressionDelimiters=('==','<','<=','>=','>');

print "Please enter the number of symbols in the constraints equations : ";
$numSymbols=<>;

print "Please enter the number of dimensions for each symbol : ";
$numDimensions=<>;

our %symbolHash;
our @symbol;
our %attributes;

print "Please enter names of the symbols in the order in which they appear in the input file : ";

our $signalRef={};
for($i=1;$i<=$numSymbols;$i++)
{
	#Modify symbol to change it from say x to $x and so on
	print "Symbol $i : ";
	$symbol[$i-1]=<>;
	chomp($symbol[$i-1]);
	$signalRef->{$symbol[$i-1]}={};
}	

print "Please enter names of attributes : ";
for($i=1;$i<=$numDimensions;$i++)
{
	print "Attribute $i: ";
	my $input=<>;
	chomp($input);
	$attributes{$input}=$i-1;
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

#tranform the constraints to convert signals to hash access and dimensions as is
&transform(@constraints);

print "Enter path to the file with values for all variables:";
$filePath=<>;

open FH, "<$filePath";

#reading in the symbol values from time series data into symbolValues (inefficient - parsing the entire data twice)
while(<FH>)
{
	push @symbolValues, [split(/\s+/,$_)];	
} 

#read in the data and store in hash of hashes
our $currentRef;
my @dimensionValues;
my $count1=0;
for($i=0;$i<=$#symbolValues;$i++){
	for($j=0;$j<=$#{$symbolValues[$i]};$j++){
		if($j<=$numDimensions-1){	#in a single line, there is dimension values and variable values.
			$dimensionValues[$j]=$symbolValues[$i][$j];				
		}
		else
		{
			#for every dimension of the signal, start assigning values from $dimensionValues array previously created
			$currentRef=$signalRef->{$symbol[$j-$numDimensions]};
			#$currentRef now pointing to first dimension hash
			for($k=0;$k<$numDimensions-1;$k++){
				if(!exists($currentRef->{$dimensionValues[$k]})){	#to handle the case where dimension value is 0	
					$currentRef->{$dimensionValues[$k]}={}; 
				}
				$currentRef=$currentRef->{$dimensionValues[$k]};
			}
			$currentRef->{$dimensionValues[$numDimensions-1]}=$symbolValues[$i][$j];
			$count1++;
		}
	}			
}

print "starting parsing entire structure $count1\n";
#parse the entire data structure
for($i=0;$i<$numSymbols;$i++){
	&recurse($signalRef->{$symbol[$i]});
}

#Constraint checking
our @varValues;
our $count=0;
our @lhsRhs=();

#number of constraints that every signal satisfies is determined
for($i=1;$i<$#symbolValues;$i++)
{
	#get the dimension values from the file that is being read in
	for($j=0;$j<$numDimensions;$j++){
			$dimensionValue[$j]=$symbolValues[$i][$j]; 
	}

=pod
	#For every symbol	
	for($j=0;$j<=$#{$symbolValues[$i]};$j++)
	{
		$tempSymbol=&modifySymbol($symbol[$j]);
		$tempSymbol =~ tr/'//d;	
		$varValues[$j]=$symbolValues[$i][$j];		
	}
=cut
	
	$avgMembership=0;	#to keep track of average of membership values for violated constraints
	$countViolation=0;	#to keep track of constraint violations

	#for every constraint	
	for($k=0;$k<$numConstraints;$k++)
	{
		#script to execute in eval
		my $tempConstraint=$constraints[$k];
		
		#$tempConstraint =~ s/(\$\w+)/$varValues[$varIndexMap{$1}]/g;
		#replace attributes with their values
		foreach my $attribute(keys %attributes){
			$tempConstraint =~ s/$attribute/$dimensionValue[$attributes{$attribute}]/g;
		}

		print "the new constraint is $tempConstraint\n";
		print "$signalRef->{x}->{86}->{87}\n";
		$evalValue=eval $tempConstraint;
		if($evalValue != 1)
		{
			$countViolation++;
			print "Constraint not satisfied - $tempConstraint\n";	
			for($l=0;$l<=$#expressionDelimiters;$l++)	
			{
				@lhsRhs=split($expressionDelimiters[$l],$tempConstraint);
				if($#lhsRhs>0)
				{
					last;
				}
			}	
			print "lhs and rhs are : $lhsRhs[0] , $lhsRhs[1]\n";
			$lhs=eval $lhsRhs[0];
			$rhs=eval $lhsRhs[1];
			$difference=abs($lhs-$rhs);
			$membership=1/(1+$difference);	#local satisfaction degree
			$avgMembership += $membership; 
			print "the lhs and rhs after evaluation are $lhs, $rhs, $difference and $membership\n";
		}
		else
		{
			$avgMembership++;
		}
	}
	$globalDegree=$avgMembership/$numConstraints;
	print "the global satisfaction degree is $avgMembership and $globalDegree\n";
}

=pod
#number of constraints that every signal satisfies is determined
for($i=1;$i<$#symbolValues;$i++)
{
	#For every symbol	
	for($j=0;$j<=$#{$symbolValues[$i]};$j++)
	{
		$tempSymbol=&modifySymbol($symbol[$j]);
		$tempSymbol =~ tr/'//d;	
		$varValues[$j]=$symbolValues[$i][$j];		
	}

	$avgMembership=0;	#to keep track of average of membership values for violated constraints
	$countViolation=0;	#to keep track of constraint violations

	#for every constraint	
	for($k=0;$k<$numConstraints;$k++)
	{
		#script to execute in eval
		$tempConstraint=$constraints[$k];
		$tempConstraint  =~ s/(\$\w+)/$varValues[$varIndexMap{$1}]/g;
		print "the new constraint is $tempConstraint\n";
		$evalValue=eval $tempConstraint;
		if($evalValue != 1)
		{
			$countViolation++;
			print "Constraint not satisfied - $tempConstraint\n";	
			for($l=0;$l<=$#expressionDelimiters;$l++)	
			{
				@lhsRhs=split($expressionDelimiters[$l],$tempConstraint);
				if($#lhsRhs>0)
				{
					last;
				}
			}	
			print "lhs and rhs are : $lhsRhs[0] , $lhsRhs[1]\n";
			$lhs=eval $lhsRhs[0];
			$rhs=eval $lhsRhs[1];
			$difference=abs($lhs-$rhs);
			$membership=1/(1+$difference);	#local satisfaction degree
			$avgMembership += $membership; 
			print "the lhs and rhs after evaluation are $lhs, $rhs, $difference and $membership\n";
		}
		else
		{
			$avgMembership++;
		}
	}
	$globalDegree=$avgMembership/$numConstraints;
	print "the global satisfaction degree is $avgMembership and $globalDegree\n";
}
=cut
