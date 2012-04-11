#!/usr/bin/perl 

package main;
use FindTestVariables;

#The main file executing 

my @constr;
push @constr, '$x=$y+$z';
push @constr, '$x=$y+1+$t';
push @constr, '$y=$x+1+5+$z';
push @constr, '$z=$x+$y+4';
push @constr, '$t=$t+$z';

=pod
push @constr, '$x=$z+1';
push @constr, '$x=$y+2';
push @constr, '$x=$a+1';
push @constr, '$x=$b+4';
=cut

=pod
push @constr, '$x=$x+$b';
push @constr, '$y=$y+$c';
push @constr, '$z=$a+3';
push @constr, '$a=$z+4';
=cut

#$fc=new FaultyConstraints(@constr);
#$fc->parseConstraints();

$fc=new FindTestVariables();
my $ret=$fc->wrapFaultyVar(@constr); 

my $ref=$fc->getVarsToTest();

print "final $#{$ref}\n";
for(my $i=0;$i<=$#{$ref};$i++)
{
	for(my $j=0;$j<=$#{$ref->[$i]};$j++)
	{
		print "$ref->[$i]->[$j]\t";
	}
	print "\n";
}
#returns reference to array of variables to test
#$fc->&extractVar($constraint);
