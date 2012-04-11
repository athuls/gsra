#For every file output with errors (i.e. 100 for each case), look at the base case for all elements identified. 
#Now for each error file, add the number of times the element was identified in the error files, and keep incrementing the counts for the elements in base files, 
#also identified in error file. Also keep track of elements identified in error file but not present in base file.
open FH3,">","/home/athuls89/Desktop/gsra/Fall2011/error_stats/approximate/error_stat_element_test";

for($position=0;$position<1;$position++){
	open FileBaseCase, "<", "/home/athuls89/Desktop/gsra/Fall2011/error_files/BFRAnalysis/runNoError/position$position/0";
	%originalEleIdent=();
	%errorEleIdent=();

	while($line=<FileBaseCase>){
		@tempArr=split(/\s+/,$line);	
		$originalEleIdent{$tempArr[0]}=0;
	}	
	
	for($count=0;$count<100;$count++)
	{

		open FileWithError, "<", "/home/athuls89/Desktop/gsra/Fall2011/error_files/BFRAnalysis/run0/position0/$count"; #the file with results from java

		while($line=<FileWithError>)
		{
			@tempArr=split(/\s+/, $line);
			if(exists($originalEleIdent{$tempArr[0]})){
				$originalEleIdent{$tempArr[0]}=$originalEleIdent{$tempArr[0]}+1;
			}
			elsif(exists($errorEleIdent{$tempArr[0]})){
				$errorEleIdent{$tempArr[0]}=$errorEleIdent{$tempArr[0]}+1;
			}
			else{
				$errorEleIdent{$tempArr[0]}=1;
			}
		}	
		close FileWithError;
	}

	%falseNegatives=();

	open FileWithOutput, ">", "/home/athuls89/Desktop/gsra/Fall2011/error_stats/approximate/positionWiseElementIdent/$position";
		
	print FileWithOutput "Elements identified in the base and error files: \n";
	
	while(($key, $value)=each(%originalEleIdent)){
		if($value!=0){
			print FileWithOutput "$key->$value\n";	
		}
		else{
			$falseNegatives{$key}=$value;
		}
	}		
	
	print FileWithOutput "False Positives: \n";
		
	while(($key, $value)=each(%errorEleIdent)){
		print FileWithOutput "$key->$value\n";	
	}
	
	print FileWithOutput "False Negatives: \n";
		
	while(($key, $value)=each(%falseNegatives)){
		print FileWithOutput "$key->$value\n";	
	}
	
	close FileWithOutput;	
	close FileBaseCase;	
}
	
