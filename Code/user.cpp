#define L 0
#define H 10000000000
#define FILE "result.txt"
#define LINES {
		res[i]=arr1[i]+arr2[i];\
		}
#include<iostream>
using namespace std;
int main()
{
	int res[H-L];
	for (int i=L;i<=H;i++)
	{
		LINES
	}
	ofstream f;
	f.open(FILE);
	for(int i=L;i<=H;i++)
	{
		f<<res[i]<<endl;
	}
	return 0;
}
