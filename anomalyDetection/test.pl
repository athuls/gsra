#!/usr/bin/perl

eval '$a=1';
eval '$b=2';
eval '$c=3';

$d='$a==$b';
$e='$a > $b';
$f='$a < $b';
$j='$c == $a + $b';

$k='$a+$b';

$testvar=0;
$testvar += $b;
print "the test var is $testvar\n";

$answer=eval $k;
print "the ansswer is $answer\n";
	
$d=~ s/\$(\w+)/${$1}/g;
print "the constraint is $d\n";
$test=eval $d;
if($test==1)
{
	print "the test passed is : $test and the variable value is $d\n";
}

if($test!=1)
{
	print "the test failed with test values as $test and variable value as $d\n";
}
$h= eval $e;
$i= eval $f;
$k= eval $j;

print "testing split\n";
@values=split('<',"x==y");
print "size of split array is $values[0]\n";
