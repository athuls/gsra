open FH3,">","./error_stat/error_GPS/vincenti/average/error_1.csv";

for($it=0;$it<100;$it++)
{
	$SF1{$it}=0;
	$SF2{$it}=0;
}

for($count=1;$count<=10;$count++)
{
open FH1, "./error_stat/error_GPS/vincenti/run$count/error_1.csv"; #the file with results from java
#open FH2, "./java_result/result_java_4"; #the file with results from java
#open FH2, "./matlab_result/result_5.txt"; #the file with results from matlab - true results

my $lineread1, $lineread2, $result, $false,$missed, @arr1, @arr2;
my $truehit2=0, $countline1=0, $countline2=0;

@arr1=();
@arr2=();

while($lineread1=<FH1>)
{	
	$countline1++;
	@arr1=split(/\s+/,$lineread1);
#	print "yoyo";
#	print $arr1[0];
#	print "yoyo";
	$SF1{$arr1[0]}=$SF1{$arr1[0]}+$arr1[1];
	$SF2{$arr1[0]}=$SF2{$arr1[0]}+$arr1[3];
	print $SF1{$arr1[0]}," ",$SF2{$arr1[0]},"\n";
}
print "\n";
close FH1;
}

for($it=0;$it<100;$it++)
{
	print FH3 $it," ",$SF1{$it}/10," ",$SF2{$it}/10,"\n";	
}

=pod
while(($key,$value)=each(%temp1))
{
	if($temp2{$key}==1)
	{
		print $key;
		print "\n";
		$truehit2=$truehit2+1;
	}
}

while($lineread1=<FH1>)
{	seek FH2, 0, 0;
	$countline1++;
	@temp1=split(/\s+/,$lineread1);
	print "@temp1\n";
	$countline2=0;
	while($lineread2=<FH2>)
	{	$countline2++;
		@temp2=split(/\s+/,$lineread2);
		print "@temp2 \n";
		if($temp1[0] eq $temp2[0])
		{
			print "yoyo";
			$truehit2=$truehit2+1;
		}
	}
}		
=cut
close FH3;
