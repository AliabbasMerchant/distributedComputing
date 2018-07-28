#include <thread>
#include "functions.cpp"

using namespace std;

int main(int argc,char **argv)
{
	char arr[10000];
    char **port1;
    char **port2;
	
	cout<<"COMPILING THE USER FILE."<<endl;

    int a = system("g++ user.cpp -o main");
	if( a!=0 )
	{
		cout<<"FOLLOWING ERRORS FOUND IN THE FILE"<<endl;
		cout<<"PLEASE DO THE NECESSARY CHANGES AND TRY AGAIN"<<endl;
		exit(0);
	}

	cout<<"PROGRAM COMPILED."<<endl;

    char l[15];
    char h[15];
    char str[100];

    ifstream f;
    f.open("user.cpp");
	f.seekg(0);

    for(f>>str;strcmp("//~",str);f>>str)
    {
    }
    for (int k=1;k<=3;k++)
    {
            f>>l;
    }
    for (int k=1;k<=3;k++)
    {
            f>>h;
    }

    int x;
    cout<<"ENTER THE NUMBER OF SYSTEMS:";
    cin>>x;

    port1 = new char*[x-1];
    port2 = new char*[x-1];
	for(int l=0;l<x-1;l++)
	{
		port1[l] = new char [4];
		port2[l] = new char [4];
	}
    int iport1=9101;
    int iport2=9201;
    for(int y=0;y<=x-2;y++)
    {
        sprintf(port1[y],"%d",iport1);
        sprintf(port2[y],"%d",iport2);
		iport1++;
		iport2++;
    }

    long long int h1;
	long long int l1;

    stringstream divy1(h);
    divy1>>h1;
    
    stringstream divy2(l);
    divy2>>l1;

    long long c=(h1-l1)/x;

    long long low[x];
    long long high[x];
	
	char **filename;
	char **resultfile;
	filename = new char * [x];	
	resultfile = new char * [x];
	for(int p=0;p<x;p++)
	{
		filename[p] = new char [20];		
		resultfile[p] = new char [20];
	}

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
	}

	for (int j=1;j<=x;j++)
	{
        stringstream ss;
        ss<<j;
        string ss1=ss.str();
        const char * s=ss1.c_str();
        strcpy(filename[j-1],"temp");
        strcat(filename[j-1],s);
        strcat(filename[j-1],".cpp");
        strcpy(resultfile[j-1],"result");
        strcat(resultfile[j-1],s);
        strcat(resultfile[j-1],".cpp");

        ifstream f;
        f.open("user.cpp");
        f.seekg(0);

        ofstream f1;
        f1.open(filename[j-1]);

		char c;

		for(f.get(c);;f.get(c))
		{
			if(c=='/')
			{
				f.get(c);
				if(c=='/')
				{
					f.get(c);
					if(c=='~')
					{
						f1<<"//~"<<endl;
						f1<<"#define L "<<low[j-1]<<endl;
        				f1<<"#define H "<<high[j-1]<<endl;
        				f1<<"#define FILE \""<<resultfile[j-1]<<"\""<<endl;
						f1<<"//!"<<endl;						
						break;
					}
					else
					{
						f.seekg(-2,ios::cur);
						goto sos;
					}
				}
				else
				{
					f.seekg(-1,ios::cur);
					goto sos;
				}
			}
			else
				goto sos;
			sos:
			f1<<c;			
		}

		for(f>>str;strcmp(str,"//!");f>>str)
		{}
		for(f.getline(str,100);!f.eof();f.getline(str,100))
		{
			f1<<str<<endl;
		}

        f1.close();
        f.close();
    }

    cout<<"FILE TEMPLATES READY."<<endl;

	thread *t1;
	t1 = new thread [x-1];

    for(int i=0;i<x-1;i++)
    {
		t1[i] = thread(send_temps,argv[1],port1[i],filename[i]);        
    }

    char g[]="g++ ";
    strcat(g,filename[x-1]);
    system(g);
    system("./a.out");

    ofstream outfile;
    outfile.open("result.cpp",ios::out);

	for(int i=0;i<x-1;i++)
    {
		t1[i].join();
    }

	thread *t2;
	t2 = new thread [x-1];

    for(int j=0;j<x-1;j++)
    {
        t2[j] = thread(recv_res,argv[1],port2[j],resultfile[j]);
    }

	for(int i=0;i<x-1;i++)
    {
		t2[i].join();        
    }
	char c1;
	for(int k=0;k<=x-1;k++)
	{
		ifstream infile;
		infile.open(resultfile[k],ios::in);
		for(infile.get(c1);!infile.eof();infile.get(c1))
		{
		    outfile.put(c1);
		}
		infile.close();
	}

    cout<<"RESULT IS READY."<<endl;

    outfile.close();

	string str1="rm ";
	for(int l=0;l<x;l++)
	{
		system((str1 + string(filename[l])).c_str());
		system((str1 + string(resultfile[l])).c_str());		
	}

    return 0;
}
