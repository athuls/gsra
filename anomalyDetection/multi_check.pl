#!/usr/bin/perl

$dim=<STDIN>;

$var=\%dimensions;

my $ref=\%data;
my $currentRef=$ref;

for($i=0;$i<$dim;$i++){
	$currentRef->{$i}=\%temp1;	
	$currentRef=$currentRef->{$i};
}

$currentRef->{'Name'}="data";

$flag=0;
print "$ref->{0}->{1}->{Name}\n" if ($flag==1);
$t=eval '$ref->{0+1}->{Name}';
print "$t\n";
=pod
for($i=0;$i<$dim;$i++){
	for($j=0;$j<10;$j++){
		$var->{$i}->{$j}="I am in $j inside $i";		
	}
}

for($i=0;$i<$dim;$i++){
	for($j=0;$j<10;$j++){
		print "$var->{$i}->{$j}\n";
	}
}
=cut	
