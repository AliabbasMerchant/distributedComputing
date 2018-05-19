#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<sstream>
using namespace std;
int main()
{
    char l[15];
    char h[15];
    ifstream f;
    f.open("user.txt");
    f.seekg(10);
    f>>l;
    cout<<l<<endl;
    f.seekg(0);
    f.seekg(22,ios::beg);
    f>>h;
    cout<<h<<endl;
    int x;
    cout<<"ENTER THE NUMBER OF SYSTEMS:";
    cin>>x;
    long long int h1;
    ///////////////////////////
    stringstream divy1(h);// CONVERTS A STRING INTO INTEGER
    divy1>>h1;
    ////////////////////////////////
    long long int l1;
    ///////////////////////////////
    stringstream divy2(l);// CONVERTS A STRING INTO INTEGER
    divy2>>l1;
    /////////////////////////////////
    long long c=(h1-l1)/x;
    long long low[x];
    long long high[x];
    char filename[x][10];
    char resultfile[x][10];
    for (int j=1;j<=x;j++)
    {
        if(j!=x)
        {
            low[j-1]=l1;
            high[j-1]=c+low[j-1];
            l1=high[j-1]+1;
        }
        else
        {
            low[j-1]=l1;
            high[j-1]=h1;
        }
        ///////////////////////////////////////
        stringstream ss;
        ss<<j;
        string ss1=ss.str();
        const char * s=ss1.c_str();
        ////////////////////////////////////////////
        strcpy(filename[j-1],"temp");
        strcat(filename[j-1],s);
        strcat(filename[j-1],".cpp");
        strcpy(resultfile[j-1],"result");
        strcat(resultfile[j-1],s);
        strcat(resultfile[j-1],".cpp");
        ifstream f;
        f.open("user.txt");
        f.seekg(0);
        ofstream f1;
        f1.open(filename[j-1]);
        f1<<"#define L "<<low[j-1]<<endl;
        f1<<"#define H "<<high[j-1]<<endl;
        f1<<"#define FILE \""<<resultfile[j-1]<<"\""<<endl;
        char str[100];
        for (int k=1;k<=3;k++)
        {
            f.getline(str,100);
        }
        for (f.getline(str,100);strcmp(str,"")!=0;f.getline(str,100))
        {
            f1<<str<<endl;
        }
        f1.close();
        f.close();
    }
    cout<<"FILE TEMPLATES READY.";
    return 0;
}
