#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

//For generating the error message.
static void bail(const char *on_what)
{
	 if ( errno != 0 )
	{
 		fputs(strerror(errno), stderr);
 		fputs(" : ", stderr);
 	}
 	fputs(on_what,stderr);
 	fputc('\n',stderr);
 	exit(1);
}

//Sending files to the clients.
void send_temps( char ip_addr[20] , char port1[5] , char filename[15] )
{
    int z;							//For return value of various functions.
    char srvr_addr[20];				//For storing the server's IP address.
    char srvr_port1[5];				//For storing the PORT number.
    struct sockaddr_in adr_srvr;	//Structure which stores the Server related information. 
    struct sockaddr_in adr_clnt;	//Structure which stores the Client related information. 
    socklen_t len_inet; 			//For storing the sizeof() value of struct variables.
    int s; 							//Server socket.
    int c; 							//Client socket.
	char arr[1000000];				//For storing data.

	strcpy(srvr_port1,port1);		//Copy the PORT number.

	strcpy(srvr_addr,ip_addr);		//Copy the Server IP address.
	
    s = socket(AF_INET,SOCK_STREAM,0);		//Initialising the server socket.
    if ( s == -1 )
        bail("socket(2)");

	int reuse = 1;
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(const char *)&reuse,sizeof(int))<0)
		bail("setsockopt(2)");

	#ifdef SO_REUSEPORT
	if(setsockopt(s,SOL_SOCKET,SO_REUSEPORT,(const char *)&reuse,sizeof(int))<0)
		bail("setsockopt(2)");
	#endif

    memset(&adr_srvr,0,sizeof(adr_srvr));		//Vacates the structure.

    adr_srvr.sin_family = AF_INET;					//Assigning the networking family.
    adr_srvr.sin_port = htons(atoi(srvr_port1));	//Assigning the PORT number.
    if ( strcmp(srvr_addr,"*") != 0 )
    {
        adr_srvr.sin_addr.s_addr = inet_addr(srvr_addr);	//Assingning the Server IP address. 
        if ( adr_srvr.sin_addr.s_addr== INADDR_NONE )
            bail("bad address.");
    }
    else
        adr_srvr.sin_addr.s_addr = INADDR_ANY ;

    len_inet = sizeof(adr_srvr);		//Storing the size of the Server's structure.

    z = bind(s,(struct sockaddr *)&adr_srvr,len_inet);			//Binding all information with the socket.
    if ( z == -1 )
        bail("bind(2)");

    z = listen(s,10);			//Listening for the connections.
    if ( z == -1 )
        bail("listen(2)");

    cout<<"MAKING CONNECTIONS WITH THE CLIENT"<<endl;

    len_inet = sizeof(adr_clnt);		//Storing the size of the Client's structure.

    c = accept(s,(struct sockaddr *)&adr_clnt, &len_inet);		//Accepting the connections.
    if ( c == -1 )
        bail("accept(2)");
    else
        cout<<"CONNECTION ACCEPTED."<<endl;

    cout<<"REQUEST ACCEPTED."<<endl;

	while(true)				// This loop does not allows the flow to go further until it makes sure whether the file 
	{						// exists. 
		fstream infile1;
		infile1.open(filename);
		if(!infile1)
			continue;
		else
		{
			infile1.close();
			break;
		}
	}

	fstream infile3;
	infile3.open(filename,ios::in);	
	infile3.seekg(0);
	
	char ch;
	int no_chars=0;

	for(infile3.get(ch);!infile3.eof();infile3.get(ch))
	{
			no_chars++;
	}
	infile3.close();

    cout<<"UPLOADING A FILE..."<<endl;

	fstream infile2;
	infile2.open(filename,ios::in);	
	infile2.seekg(0);

    for(infile2.getline(arr,1000000);!infile2.eof();infile2.getline(arr,1000000)) 		//This loop uploads the file. 
    {
        z=write( c, arr, strlen(arr));
        if(z==-1)
            bail("write(2)");
        z=write( c, "\n" ,strlen("\n"));
        if(z==-1)
            bail("write(2)");
    }
	char num[6];

	sprintf(num,"%d",no_chars);
	no_chars=no_chars + strlen(num) + 7;
	sprintf(num,"%d",no_chars);

	char init_arr[50] = "//^ = ";
	strcat(init_arr,num);
	strcat(init_arr,"/");
	
	z=write( c,init_arr,strlen(init_arr));
    if(z==-1)
        bail("write(2)");
    z=write( c, "\n" ,strlen("\n"));
    if(z==-1)
        bail("write(2)");

	cout<<"UPLOADING COMPLETE."<<endl;
    cout<<"CLOSING SOCKET..."<<endl;
    close(c);								//Closing the Client socket.
    close(s);								//Closing the Server socket.
	
	infile2.close();
}

//Taking the result files from the clients.
void recv_res(char ip_addr[20],char port2[5],char resultfile[15])
{
	char chp = port2[3]; 
	int count= 0;
	fstream res;
	res.open(resultfile,ios::out);
	res.seekg(0);
	int flag = 0;
	long long int arr_len;
	cout<<"MAKING CONNECTIONS WITH THE CLIENT "<<chp<<endl;
	do
	{
		count++;
		int z;									//For return value of various functions.
		char srvr_addr[20];						//For storing the server's IP address.
		char srvr_port2[5];						//For storing the PORT number.
		struct sockaddr_in adr_srvr;			//Structure which stores the Server related information. 
		struct sockaddr_in adr_clnt;			//Structure which stores the Client related information. 
		socklen_t len_inet; 					//For storing the sizeof() value of struct variables.
		int s; 									//Server socket.
		int c; 									//Client socket.
		char arr[8000000];						//For storing data.

		strcpy(srvr_port2,port2);			//Copy the PORT number.

		int port = atoi(port2);
		port++;
		sprintf(port2,"%d",port);

		strcpy(srvr_addr,ip_addr);				//Copy the Server IP address.

		s = socket(AF_INET,SOCK_STREAM,0);			//Initialising the server socket.
		if ( s == -1 )
			bail("socket()");

		int reuse = 1000;
		if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(const char *)&reuse,sizeof(int))<0)
			bail("setsockopt(2)");

		memset(&adr_srvr,0,sizeof(adr_srvr));			//Vacates the structure.

		adr_srvr.sin_family = AF_INET;						//Assigning the networking family.
		adr_srvr.sin_port = htons(atoi(srvr_port2));		//Assigning the PORT number.
		if ( strcmp(srvr_addr,"*") != 0 )
		{
			adr_srvr.sin_addr.s_addr = inet_addr(srvr_addr);	//Assingning the Server IP address. 	
			if ( adr_srvr.sin_addr.s_addr== INADDR_NONE )
				bail("bad address.");
		}
		else
			adr_srvr.sin_addr.s_addr = INADDR_ANY ;

		len_inet = sizeof(adr_srvr);			//Storing the size of the Server's structure.

		while(true)
		{
			z = bind(s,(struct sockaddr *)&adr_srvr,len_inet);				//Binding all information with the socket.
			if ( z == -1 )
				continue;
			else
				break;
		}

		z = listen(s,10);						//Listening for the connections.
		if ( z == -1 )
			bail("listen(2)");

		len_inet = sizeof(adr_clnt);			//Storing the size of the Client's structure.

	
		c = accept(s,(struct sockaddr *)&adr_clnt, &len_inet);			//Accepting the connections.
		if ( c == -1 )
        	bail("accept(2)");
		else
			if(count==1)
			{
				cout<<"CONNECTION FOR CLIENT "<<chp<<" ACCEPTED."<<endl;
				cout<<"ORDERING TO A CLIENT "<<chp<<" FOR A FILE..."<<endl;
				cout<<"DOWNLOADING A FILE FROM CLIENT "<<chp<<"..."<<endl;
			}
	
		
		usleep(50000);
		read(c,arr,sizeof(arr)-1);					//Downloading a result file.

		arr_len = strlen(arr);
		if(count == 1)
			cout<<"RECEIVED "<<count<<" PACKET FROM CLIENT "<<chp<<endl;
		else
			cout<<"RECEIVED "<<count<<" PACKETS FROM CLIENT "<<chp<<endl;		//TODO
		if(arr[arr_len-3]!='&')
		{
			for(int l=strlen(arr)-1;l>=0;l--)
			{
				if(arr[l]=='^')
				{
					strcpy(arr,string(arr).substr(0,l-2).c_str());
					flag = 1;
					break;
				}
			}
		}
		else
			strcpy(arr,string(arr).substr(0,arr_len-5).c_str());

		res<<arr;
	
		close(c);					//Closing the Client socket.
		close(s);					//Closing the Server socket.
 		  		
	}while(flag==0);
	cout<<"DOWNLOADING COMPLETE."<<endl;
	cout<<"CLOSING SOCKET..."<<endl;
	res.close();
}	
