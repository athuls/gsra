#For every file output with errors (i.e. 100 for each case), look at the base case for all elements identified. 
#Now for each error file, add the number of times the element was identified in the error files, and keep incrementing the counts for the elements in base files, 
#also identified in error file. Also keep track of elements identified in error file but not present in base file.
open FH3,">","/home/athuls89/Desktop/gsra/Fall2011/error_stats/approximate/error_stat_element_test_del";

for($count=0;$count<100;$count++)
{
open FH1, "/home/athuls89/Desktop/gsra/Fall2011/error_files/approximate/after_mod/unique_elements/output_unique_elements$count"; #the file with results from java
#open FH2, "./java_result/result_java_4"; #the file with results from java
#open FH2, "/home/athuls89/Desktop/gsra/matlab_result/result_5.txt"; #the file with results from matlab - true results
open FH2, "/home/athuls89/Desktop/gsra/Fall2011/error_files/approximate/after_mod/unique_elements/without_error"; 

my $lineread1, $lineread2, $result, $false,$missed, %temp1, %temp2, @arr1, @arr2;
my $truehit2=0, $countline1=0, $countline2=0;

@arr1=();
@arr2=();
%temp1=();
%temp2=();
%elementsIdentified=();

while($lineread1=<FH1>)
{	
	$countline1++;
	@arr1=split(/\s+/,$lineread1);
	$temp1{$arr1[0]}=$arr1[1];
}
print "\n";
while($lineread2=<FH2>)
{
	$countline2++;
	@arr2=split(/\s+/,$lineread2);
	$temp2{$arr2[0]}=1;
}

while(($key,$value)=each(%temp1))
{
	if($temp2{$key}==1)
	{
		print $key;
		print "\n";
		$elementsIdentified{$key}=$value;
		$truehit2=$truehit2+1;
	}
}

$truehit2=scalar(keys %elementsIdentified);

=pod
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

$result=$truehit2/$countline1;
$false=($countline1-$truehit2)/$countline1;
$missed=($countline2-$truehit2)/$countline2;
print "$truehit2  $countline1 $countline2";
print " $result  $false  $missed";

print FH3 $count, " ", $result, " ",$false," ",$missed, " ";
while(($key, $value)=each(%elementsIdentified)){
	print FH3 $key, "->", $value, " ";
}
print FH3 "\n";

close FH1;
close FH2;

}
close FH3;
close FH2;
close FH1;
