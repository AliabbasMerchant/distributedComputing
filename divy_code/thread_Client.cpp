#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include<fstream>
#include<string>

#define PORT1 "9101"
#define PORT2 "9201"

using namespace std;

static void bail(const char *on_what)
{
 	if ( errno != 0 )
	{
 		fputs(strerror(errno),stderr);
 		fputs(": ",stderr);
	}
 	fputs(on_what,stderr);
 	fputc('\n',stderr);
	exit(1);
}

int main(int argc,char **argv)
{
    fstream outfile;
    outfile.open("temp.cpp",ios::out);

 	int z;
 	char srvr_addr[20];
 	char srvr_port1[5];
 	struct sockaddr_in adr_srvr;
 	socklen_t len_inet; 			
 	int s; 				
 	char dtbuf[8000000];

	strcpy(srvr_port1,PORT1);

	if ( argc >= 2 )
 		strcpy(srvr_addr,argv[1]);
	else
 		strcpy(srvr_addr,"127.0.0.1");

	s = socket(AF_INET,SOCK_STREAM,0);
 	if ( s == -1 )
 		bail("socket()");

 	memset(&adr_srvr,0,sizeof(adr_srvr));

 	adr_srvr.sin_family = AF_INET;
 	adr_srvr.sin_port = htons(atoi(srvr_port1));
 	adr_srvr.sin_addr.s_addr = inet_addr(srvr_addr);
 	if ( adr_srvr.sin_addr.s_addr == INADDR_NONE )
		bail("bad address.");
	len_inet = sizeof(adr_srvr);

    cout<<"MAKING CONNECTIONS WITH THE SERVER."<<endl;

	while(true)
	{
		z = connect(s,(struct sockaddr*)&adr_srvr,len_inet);
	 	if ( z == -1 )
			continue;
		else
		{
			cout<<"CONNECTED TO SERVER"<<endl;
			break;
		}
	}

	cout<<"MAKING REQUEST FOR THE FILE."<<endl;
	cout<<"DOWNLOADING A FILE..."<<endl;

	usleep(50000);

    z=read(s,dtbuf,sizeof(dtbuf)-1);

	int l_acc;
	int f_acc;
	
	for(int k = 0;k < sizeof(dtbuf)-1; k++ )
	{
		if(dtbuf[k]=='/')
		{
			if( (k++) && (dtbuf[k]=='/') )
			{
				if( (k++) && (dtbuf[k]=='^') )
				{
					f_acc = k;
					k++;
					while(!(dtbuf[k]=='/'))
					{
						k++;
					}
					l_acc=k;
				}
			}
		}
	}
	char num[5]; 
	strcpy(num , string(dtbuf).substr((f_acc + 4),(l_acc - f_acc - 4)).c_str());
	int num_chars = atoi(num);	
	
	strcpy(dtbuf,string(dtbuf).substr(0,num_chars).c_str());
    outfile << dtbuf;
	
    outfile.close();

    cout<<"DOWNLOADING COMPLETE."<<endl;
    cout<<"CLOSING SOCKET..."<<endl;

 	close(s);

	system("g++ temp.cpp -o tem");
	system("./tem");

	char result[50];

	fstream res;
	res.open("temp.cpp",ios::in);

	for(res>>result;strcmp(result,"//~");res>>result)
	{
	}
	res.getline(result,49);
	res.getline(result,49);
	res.getline(result,49);
	res>>result;
	res>>result;
	res>>result;

	int acc = strlen(result)-2;
	strcpy(result,(char *)string(result).substr(1,acc).c_str());

	ifstream infile1;
	infile1.open(result,ios::in);
	
	long long int no_chars=0;
	infile1.seekg(0,ios::end);
	no_chars = infile1.tellg();

	infile1.close();
	int port;
	char port2[5];	
	long long int no_read=0;
	strcpy(port2,PORT2);
	int count=0;
	ifstream infile;
	infile.open(result,ios::in);
	cout<<"MAKING CONNECTIONS WITH THE SERVER."<<endl;
	do
	{
		count++;
		ifstream infile;
		infile.open(result,ios::in);	
		infile.seekg(no_read);
	 	char srvr_port2[5];

		strcpy(srvr_port2,port2);

		port = atoi(port2);
		port++;
		sprintf(port2,"%d",port);

		if ( argc >= 2 )
	 		strcpy(srvr_addr,argv[1]);
		else 
	 		strcpy(srvr_addr,"127.0.0.1");

		s = socket(AF_INET,SOCK_STREAM,0);
	 	if ( s == -1 )
	 		bail("socket()");

	 	memset(&adr_srvr,0,sizeof(adr_srvr));

	 	adr_srvr.sin_family = AF_INET;
	 	adr_srvr.sin_port = htons(atoi(srvr_port2));
	 	adr_srvr.sin_addr.s_addr = inet_addr(srvr_addr);
	 	if ( adr_srvr.sin_addr.s_addr == INADDR_NONE )
			bail("bad address.");
		len_inet = sizeof(adr_srvr);

	 	while(true)
		{
			z = connect(s,(struct sockaddr*)&adr_srvr,len_inet);
		 	if ( z == -1 )
				continue;
			else
			{
				if(count==1)
					cout<<"CONNECTED TO SERVER"<<endl;
				break;
			}
		}

		if(no_chars<=849989)
		{
			infile.getline(dtbuf,no_chars+1);
			no_read = no_read + strlen(dtbuf);
			strcat(dtbuf,"/*^*/");
			write(s,dtbuf,strlen(dtbuf));	
			if(count == 1)
				cout<<"SENT "<<count<<" PACKET."<<endl;
			else
				cout<<"SENT "<<count<<" PACKETS."<<endl;
			infile.close();
			close(s);
			break;
		}
		else
		{
			infile.getline(dtbuf,850000+1);
			no_read = no_read + strlen(dtbuf);
			no_chars = no_chars - strlen(dtbuf);
			strcat(dtbuf,"/*&*/");
			write(s,dtbuf,strlen(dtbuf));
			if(count == 1)
				cout<<"SENT "<<count<<" PACKET."<<endl;
			else
				cout<<"SENT "<<count<<" PACKETS."<<endl;				
			infile.close();
			close(s);
		}

	}while(true);

	cout<<"RESULT SENT TO THE SERVER."<<endl;
	return 0;
}
