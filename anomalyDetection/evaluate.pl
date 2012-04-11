#!/usr/bin/perl

@arr1=(1,2,3,4,5,6,7,8,9,10);
@arr2=(2,3,4,5,6,7,8,9,10,11);

$expr= '$s1[$t]=$s2[$t-1]';

$t=2;
@s1=@arr1;
@s2=@arr2;
eval $expr;
print "$s1[$t]\n";

for($i=0;$i<$#arr1;$i++)
{
#	if(eval $arr1[$i])
}

$temp1='$c=($a<=$b)';
$a=20;
$b=1;
eval "$temp1";
print "$c\n"
