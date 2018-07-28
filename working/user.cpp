#include<iostream>

int func(int a)
{
    int b = a * 2;
    return b;
}

int main()
{
    int a[] = {55,77,66,88};
    int b[(sizeof(a)/sizeof(a[0]))];
    for(int i=0; i<(sizeof(a)/sizeof(a[0])); i++)
    {
        a[i] = func(a[i]);
    }
    return 0;
}
