#include<iostream>
#include<fstream>
//~
#define L 0
#define H 9999999
#define FILE "result.cpp"
//!
using namespace std;
int main()
{
	int *arr1;
	int *arr2;
	int *res;
	unsigned int size=(H+1);
	arr1 = new int [size];
	arr2 = new int [size];
	res = new int [size];
	for(int i=L;i<=H;i++)
	{
		arr1[i]=i+1;
		arr2[i]=i+1;
	}
	for (int i=L;i<=H;i++)
	{
		res[i]=arr1[i]+arr2[i];
	}
	ofstream f;
	f.open(FILE,ios::out);
	for(int i=L;i<=H;i++)
	{
		f<<res[i]<<",";
	}
	f.close();
	return 0;
}
