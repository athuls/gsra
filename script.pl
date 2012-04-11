open FH1, "result_file"; #the file with results from java
open FH2, "result_file"; #the file with results from matlab - true results

my $lineread1, $lineread2, $result, $false,$missed, @temp1, @temp2;
my $truehit2=0, $countline1=0, $countline2=0;

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

$result=$truehit2/$countline1;
$false=($countline1-$truehit2)/$countline1;
$missed=($countline2-$truehit2)/$countline2;
print "$truehit2  $countline1 $countline2";
print " $result  $false  $missed";

