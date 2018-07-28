#include<iostream>
#include<fstream>
#include<cstring>
using namespace std;

int generate_req_files(char user[], char num[], char temp[])
{
	int pos;
	ifstream f(user,ios::in);
	ofstream f1(temp,ios::out), f2(num,ios::out);
	char c[100],copy[100];
	int p[13],p_[13],p__[13],flag=0,flagg=0,i=0,check=0,checkk=0,token=0;
	p[0]=0;
	p[5]=75;
	p[3]=119;
	p[4]=125;
	p_[4]=p[4];
	p_[3]=p[3];
	p__[0]=p[0];
	p__[3]=p[3];
	p__[4]=p[4];
	
	while(!f.eof()&&check==0)
	{
		if(p[i]==flag)
			{
				switch(i)
				{
					case 0: 
							f1<<"#include<fstream>"<<endl<<"#include<stdlib.h>"<<endl<<"#include"<<"\"helper_funcs.h\""<<endl;
							p[5]+=63;
							p[3]+=63;
							p[4]+=63;
							flag+=63;
							i=5;
							//check=-1;
							break;
					case 5:
//							f1<<"int argc, char * argv[2]";
							pos = f1.tellp();
							f1.seekp(pos-2);
							f1.write(" ",1);
							f1<<"int argc, char * argv[2])";
							p[3]+=24;
							p[4]+=24;
							flag+=24;
							flag+=p_[5];
							i=3;
							//check=-1;
							break;
					case 3:
						
					case 4: 
							f1<<"{"<<endl<<"\tint i = atoi(argv[1]);"<<endl<<"\ta[i] = result_store(func(a[i]));"<<endl<<"}"<<endl<<"return 0;"<<endl<<"}";
							check=-1;
							while(!f.eof())
							{
								f>>c;					}						
				}
			}
		else 
			{
				f.getline(c,100);
					
				f1<<c<<endl;
				
				flag+=strlen(c)+1;
				if(flag>p[5]&&token<2)
				{
					if(flag>p[3])
					{
						p_[3]=flag-p[3];
						flag=p[3];
						token++;
					
					}
					else if(!token)	
				 		{
				 			p_[5]=flag-p[5];
				 			flag=p[5];
							token++;
						}		
				}
			}
	
    }
	f.close();
	f.open(user,ios::in);
    i=0;
    while(!f.eof()&&checkk==0)
    {
    	if(p__[i]==flagg)
    	{
    		switch(i)
    		{
    			case 0:
						f2<<"#include<fstream>"<<endl<<"#include\"helper_funcs.h\""<<endl;
						p__[3]+=42;
						p__[4]+=42;
						i=3;
						flagg+=42;
						break;
				case 3: f.getline(copy,100);
						f2<<"\tstd::fstream file(\"dc/list.txt\", std::ios::out);"<<endl<<copy<<endl;
				case 4:
						f2<<"\t{\n\t\tfile<<i<<std::endl;"	<<endl<<"\t}"<<endl<<"file.close();"<<endl<<"exit(0);"<<endl;
						f2<<copy<<endl;
						f2<<"\t{\n"<<"\t\t a[i] = func(a[i]);"<<endl<<"\t}"<<endl<<"return 0;"<<endl<<"}";
						checkk=-1;
			}
		}
		
		else
		{
			f.getline(c,100);
			f2<<c<<endl;
			flagg+=strlen(c)+1;
            if(flagg>p__[3])
            flagg-=flagg-p__[3];
		}
	}
	return 0;

}
