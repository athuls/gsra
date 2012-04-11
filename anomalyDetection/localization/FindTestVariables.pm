#!/usr/bin/perl

package FindTestVariables;

use FaultyConstraints;

sub new 
{
	my @passedArg;

        #get the name of the class and array of arguments containing constraints
        my ($class,@passedArg)=@_;

        my $self={
		#returns an array of references to arrays i.e. set of sets
		"returnSet" => [],		
		"variablesTested" => {}
	};

        bless $self, $class;
        return $self;
}

#wrapper subroutine for faultyVar
sub wrapFaultyVar
{
	my ($self)=@_;
		
        my @constrSet;
                
        #Get all the constraints over which faults have to be determined
        for(my $i=1;$i<=$#_;$i++)
        {
                push @constrSet, $_[$i];                   
        }
		
	my $retSet = $self->faultyVar(@constrSet);
	
	if($#{$retSet}!=-1)
	{
		push @{$self->{"returnSet"}}, $retSet;
	}
	
	while($#{$retSet}!=-1)
	{	
		for(my $j=0;$j<=$#{$retSet};$j++)
		{		
			$self->{"variablesTested"}->{$retSet->[$j]}++;	
		}
		$retSet = $self->faultyVar(@constrSet);
		if($#{$retSet}!=-1)
		{
			push @{$self->{"returnSet"}}, $retSet;
		}

	}
}

#Subroutine to check for variables with max span and return them for testing. 
#Returns reference to array with variables to test
sub faultyVar
{       
        my ($self)=@_;
 
        my @violatedConstr;
                
        #Get all the constraints over which faults have to be determined
        for($i=1;$i<=$#_;$i++)
        {
                push @violatedConstr, $_[$i];                   
        }
        
       	my $faultyConstr=new FaultyConstraints(@violatedConstr);
        $faultyConstr->parseConstraints();
        
        #sort hash of variables by value and if the top most variable does not span the length of constraints, take the max and call this function again over the remaining set of constraints, merge the two sets of variables!!
               
	#Sort by number of constraints that each variable covers 
        #foreach $key (sort {($#{$faultyConstr->{"variables"}->{$b}}+1) <=> ($#{$faultyConstr->{"variables"}->{$a}}+1)} (keys %{$faultyConstr->{"variables"}}))
        #{

	#@sortedVar=sort {($#(keys %{$faultyConstr->{"variables"}->{$b}})) <=> ($#(keys %{$faultyConstr->{"variable"}->{$a}}))} (keys %{$faultyConstr->{"variables"}});

	my @sortedVar=sort {keys %{$faultyConstr->{"variables"}->{$b}} <=> keys %{$faultyConstr->{"variables"}->{$a}}} keys %{$faultyConstr->{"variables"}};

	my $key, $varCount=0;

	print "sorted variables\n";
	for(my $i=0;$i<=$#sortedVar;$i++)
	{
		my $size=keys %{$faultyConstr->{"variables"}->{$sortedVar[$i]}};
		print "$sortedVar[$i]\t$size\n";
	}
	print "/sorted variables\n";

	do	
	{
		if($varCount>$#sortedVar)
		{
			return [];
		}
		$key=$sortedVar[$varCount];
		$varCount++;
	}while(exists($self->{"variablesTested"}->{$key}));

		my $constraintSpan=keys %{$faultyConstr->{"variables"}->{$key}};
                if($constraintSpan == $faultyConstr->{"constraintCount"})
                {
                	return [$key];
                }
                else
                {
                        my @complementaryConstr, @varToTest;
                        #For every violated constraint, check if the topmost variable appears in the constraint, else add it to the complementary set 
                        for(my $i=0;$i<=$#violatedConstr;$i++)
                        {
                                if(!exists($faultyConstr->{"variables"}->{$key}->{$violatedConstr[$i]}))
				{
					push @complementaryConstr, $violatedConstr[$i];
				}
                        }
		
			my $retRef=$self->faultyVar(@complementaryConstr);
			if($#{$retRef}!=-1)
			{
				push @{$retRef}, $key;			 
				return $retRef;
			}
			else
			{
				return [];
			}
                }
}

#returns a reference to the array containing reference to arrays
sub getVarsToTest
{
	my ($self) = @_;
	return $self->{"returnSet"};
}

1;
