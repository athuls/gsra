#!/usr/bin/perl

package FaultyConstraints;

sub new
{
	
	my @passedArg;
	
	#get the name of the class and array of arguments containing constraints
	my ($class,@passedArg)=@_; 
	
	my $self={};

	#stores the constraints along with the variables that appear and their counts
	$self->{"constraints"}={};

	my $count=0;
	while($count<=$#passedArg)
	{
		$self->{"constraints"}->{$passedArg[$count]}={};
		$count++;
	}	

	my $constraintCount=keys(%{$self->{"constraints"}});
	$self->{"constraintCount"}=$constraintCount;	

	#Stores the variables along with the array of constraints in which it appears
	$self->{"variables"}={};	
				
	bless $self, $class;	
	return $self;
}

#Does not return anything. Parses the constraints and fills in the constraints hash with references to arrays of variables in corresponding constraints
sub parseConstraints
{
	my ($self)=@_;
	
	for my $key (keys %{$self->{"constraints"}})
	{
		$self->{"constraints"}->{$constr}=$self->extractVar($key);
	}
}

#[Internal/Helper Subroutine] Returns the reference to hash with with variables for constraint passed as argument
sub extractVar
{
	my ($self,$constr)=@_;
	my $extractedVar={};
		
		while($constr =~ m/(\$[a-zA-Z0-9]+)/g)
		{
			$extractedVar->{$1}++;
		}

		#@temp=($constr=~m/([a-zA-Z0-9]+)/g);
		for(keys %{$extractedVar})
		{
			#$self->{"variables"}->{$_}++;
			#if(!exists($self->{"variables"}->{$_}))
			#{
			#	$self->{"variables"}->{$_}={};
			#}
			$self->{"variables"}->{$_}->{$constr}++;
		}
		return $extractedVar;
}

#helper function for sorting hash by value
sub hashValueDescendingNum {
   $grades{$b} <=> $grades{$a};
}

#print out contents of variables
sub printOut
{
	my ($self)=@_;

	my $size=keys %{$self->{"variables"}};
	print "I am in print out\t$size\n";
	for my $key (keys %{$self->{"variables"}})
	{
		print "$key\t";
		for my $key(keys %{$self->{"variables"}->{$key}})
		{
			print "$key\t";
		}
		print "\n";
	}
} 

sub getConstraint
{
	my ($self)=@_;
	return $self->{$_[1]};
}
1;
