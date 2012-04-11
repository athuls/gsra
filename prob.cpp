#include <stdlib.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <map>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include<math.h>

using namespace std;

float weibull(float x)
{
	return(2*x/exp(x*x));
}

float max(float num[])
{
	float temp=weibull(num[0]);
	int ret=0;
	for(int i=0;i<5;i++)
	{	if(temp<weibull(num[i]))
		{	temp=weibull(num[i]);
			ret=i;
		}
	}	
	return num[ret];
}

int main()
{
	srand(time(NULL));
	float r[5];
	for(int j=0;j<1000;j++)
	{
	for(int i=0;i<5;i++)
		r[i]=5*((double)rand()/(double)RAND_MAX);
	cout<<max(r)<<endl;
	}
	
	
}
