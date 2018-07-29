#include<iostream>
// some include statements
//using namespace std;

// __CIF__
int func(int a)
{
    int b = int(a / 3) + 551;
    
    
    
    return b;
}




int main()
{


    int a[] = {5005,707,366,88, 88};
    int b[(sizeof(a)/sizeof(a[0]))];
// __LOOP__
    for(int i=0; i<(sizeof(a)/sizeof(a[0])); i++)
    {
        a[i] = func(a[i]);
    }
    return 0;
}
