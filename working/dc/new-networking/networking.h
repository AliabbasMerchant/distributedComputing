#include<iostream>
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
#include <fstream>
#include<cstring>
#include<cstdlib>
#include <string>
#include <sstream>
using namespace std;

//char ip_address[] = "172.16.100.61";
static int bail(const char *on_what)
{
    ////cout<<"Bail"<<endl;
	if ( errno != 0 )
	{
 		fputs(strerror(errno), stderr);
 		fputs(": ", stderr);
 	}
 	fputs(on_what,stderr);
 	fputc('\n',stderr);
 	// exit(1);
    if(strcmp("bind(2)", on_what)==0)
        return 2;
    return 1;
}

int send_command(const char PORT[6], const char command[100], char ip_address[18])
{
    int z;
 	char srvr_addr[20], srvr_port[6];
 	strcpy(srvr_port,PORT);
	struct sockaddr_in adr_srvr;	/* AF_INET */
	struct sockaddr_in adr_clnt;	/* AF_INET */
 	socklen_t len_inet; 			/* length */
 	int s; 				/* Socket */
 	int c; 				/* Client socket */
    
    if(ip_address!=NULL)
    	strcpy(srvr_addr,ip_address);
    else
     	strcpy(srvr_addr,"127.0.0.1");

	s = socket(AF_INET,SOCK_STREAM,0);
	if ( s == -1 ) {
		//return bail("socket()");
		bail("socket()");
	}
	int reuse = 1;
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(const char *)&reuse,sizeof(int))<0)
		return bail("setsockopt(2)");
	#ifdef SO_REUSEPORT
	if(setsockopt(s,SOL_SOCKET,SO_REUSEPORT,(const char *)&reuse,sizeof(int))<0)
		bail("setsockopt(2)");
	#endif
	

	memset(&adr_srvr,0,sizeof(adr_srvr));
	adr_srvr.sin_family = AF_INET;
	adr_srvr.sin_port = htons(atoi(srvr_port));
	if ( strcmp(srvr_addr,"*") != 0 )
	{
		adr_srvr.sin_addr.s_addr = inet_addr(srvr_addr);
		if ( adr_srvr.sin_addr.s_addr== INADDR_NONE ) {
			close(s);
			return bail("bad address.");
		}
	}
	else
	{
		adr_srvr.sin_addr.s_addr = INADDR_ANY ;
	}
	len_inet = sizeof(adr_srvr);
	while(true) {
    	z = bind(s,(struct sockaddr *)&adr_srvr,len_inet);
    	if(z==-1) {
    	    cout<<"Bail"<<endl;continue;
    	}
    	else {
    	    break;
	    }
	}
	//if ( z == -1 ) {
	//	close(s);
	//	return bail("bind(2)");
	//}
	z = listen(s,10);
	if ( z == -1 ) {
		close(s);
		return bail("listen(2)");
	}

	//cout<<"MAKING CONNECTIONS WITH THE CLIENT"<<endl;
    len_inet = sizeof(adr_clnt);
	
	c = accept(s,(struct sockaddr *)&adr_clnt, &len_inet);
    if ( c == -1 )
	{
		close(s);
        return bail("accept(2)");
	}
    else
    {
        //cout<<"CONNECTION ACCEPTED."<<endl;
    }
    /* BRAIN OF THE PROGRAM*/
        z=write( c, command, strlen(command));
        if(z==-1) {
			close(c);
			close(s);
            return bail("write(2)");
		}
    
	usleep(1*1000);
    close(c);
    //cout<<"Closing socket..."<<endl;
    close(s);
	return 0;
}


string send_command(const char PORT[], const char command[], char ip_address[], bool get_client_ip)
{
    //cout<<PORT<<command<<ip_address<<endl;
	int z;
 	char srvr_addr[20], srvr_port[6];
 	strcpy(srvr_port,PORT);
	struct sockaddr_in adr_srvr;	/* AF_INET */
	struct sockaddr_in adr_clnt;	/* AF_INET */
 	socklen_t len_inet; 			/* length */
 	int s; 				/* Socket */
 	int c; 				/* Client socket */
    
    if(ip_address!=NULL)
    	strcpy(srvr_addr,ip_address);
    else
     	strcpy(srvr_addr,"127.0.0.1");

    s = socket(AF_INET,SOCK_STREAM,0);
	if ( s == -1 ) {
		//return bail("socket()");
		bail("socket()");
	}
	int reuse = 1;
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(const char *)&reuse,sizeof(int))<0) {
		bail("setsockopt(2)");
		return "ERROR";
	}
	#ifdef SO_REUSEPORT
	if(setsockopt(s,SOL_SOCKET,SO_REUSEPORT,(const char *)&reuse,sizeof(int))<0)
		bail("setsockopt(2)");
	#endif
	
	memset(&adr_srvr,0,sizeof(adr_srvr));
	adr_srvr.sin_family = AF_INET;
	adr_srvr.sin_port = htons(atoi(srvr_port));
	if ( strcmp(srvr_addr,"*") != 0 )
	{
		adr_srvr.sin_addr.s_addr = inet_addr(srvr_addr);
		if ( adr_srvr.sin_addr.s_addr== INADDR_NONE ) {
			close(s);
			bail("bad address.");
			return "ERROR";
		}
	}
	else
	{
		adr_srvr.sin_addr.s_addr = INADDR_ANY ;
	}
	len_inet = sizeof(adr_srvr);
	while(true) {
    	z = bind(s,(struct sockaddr *)&adr_srvr,len_inet);
    	if(z==-1) {
    	    cout<<"Bail"<<endl;continue;
    	}
    	else {
    	    break;
	    }
	}
	//if ( z == -1 ) {
	//	close(s);
	//	bail("bind(2)");
	//  return "ERROR";
	//}
	
	z = listen(s,10);
	if ( z == -1 ) {
		close(s);
		bail("listen(2)");
		return "ERROR";
	}
	//cout<<"MAKING CONNECTIONS WITH THE CLIENT"<<endl;
    len_inet = sizeof(adr_clnt);
	
	c = accept(s,(struct sockaddr *)&adr_clnt, &len_inet);
    if ( c == -1 ) {
		close(s);
        bail("accept(2)");
		return "ERROR";
	}
    else
    {
        //cout<<"CONNECTION ACCEPTED."<<endl;
    }

	struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&adr_clnt;
	struct in_addr ipAddr = pV4Addr->sin_addr;
	char client_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipAddr, client_ip, INET_ADDRSTRLEN);

    /* BRAIN OF THE PROGRAM*/
        z=write( c, command, strlen(command));
        if(z==-1) {
			close(c);
			close(s);
            bail("write(2)");
			return "ERROR";
		}
	usleep(1*1000);
    close(c);
    //cout<<"Closing socket..."<<endl;
    close(s);
	return string(client_ip);
}

string receive_command(const char PORT[], char ip_address[])
{
    string command;
 	int z;
 	char srvr_addr[20], srvr_port[6];
 	strcpy(srvr_port,PORT);
 	struct sockaddr_in adr_srvr;	/* AF_INET */
 	socklen_t len_inet; 			/* length */
 	int s; 				/* Socket */
 	char dtbuf[2000];
    if(ip_address!=NULL)
    	strcpy(srvr_addr,ip_address);
    else
     	strcpy(srvr_addr,"127.0.0.1");

	s = socket(AF_INET,SOCK_STREAM,0);
	if ( s == -1 ) {
		//return bail("socket()");
		bail("socket()");
	}
	int reuse = 1;
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(const char *)&reuse,sizeof(int))<0) {
		bail("setsockopt(2)");
		return "___";
	}
	#ifdef SO_REUSEPORT
	if(setsockopt(s,SOL_SOCKET,SO_REUSEPORT,(const char *)&reuse,sizeof(int))<0)
		bail("setsockopt(2)");
	#endif
	
 	memset(&adr_srvr,0,sizeof(adr_srvr));
 	adr_srvr.sin_family = AF_INET;
 	adr_srvr.sin_port = htons(atoi(srvr_port));
 	adr_srvr.sin_addr.s_addr = inet_addr(srvr_addr);
 	if ( adr_srvr.sin_addr.s_addr == INADDR_NONE )
    {
		close(s);
		bail("bad address.");
        return "___";
    }
	len_inet = sizeof(adr_srvr);
    //cout<<"MAKING CONNECTIONS WITH THE SERVER."<<endl;
 	z = connect(s,(struct sockaddr*)&adr_srvr,len_inet);
 	if ( z == -1 )
    {
		close(s);
		bail("connect(2)");
        return "___";
    }
	else
		//cout<<"CONNECTED TO SERVER"<<endl;
	*dtbuf = NULL;
	/* BRAIN OF THE PROGRAM*/
    z=read(s,dtbuf,sizeof(dtbuf)-1);
    string str =string(dtbuf);
    str=str.substr(0, str.size());
    command = str;
    if(z==-1)
    {
		close(s);
        bail("read(2)");
        return "___";
    }
    //cout<<"CLOSING SOCKET..."<<endl;
	usleep(1*1000);
 	close(s);
	return command;
}

int obj_send(const char PORT1[], const char PORT2[], const char filename[], char ip_address[])
{
    fstream infile;
    infile.open(filename,ios::in|ios::binary);
	int z;
 	char srvr_addr[20]; /*CORRECTION*/
 	char srvr_port1[6];
	strcpy(srvr_port1, PORT1);
	struct sockaddr_in adr_srvr;	/* AF_INET */
	struct sockaddr_in adr_clnt;	/* AF_INET */
 	socklen_t len_inet; 			/* length */
 	int s; 				/* Socket */
 	int c; 				/* Client socket */
	if(ip_address!=NULL)
    	strcpy(srvr_addr,ip_address);
    else
     	strcpy(srvr_addr,"127.0.0.1");
	//cout<<"trying to open socket"<<endl;
	
	
	s = socket(AF_INET,SOCK_STREAM,0);
	if ( s == -1 ) {
		//return bail("socket()");
		bail("socket()");
	}
	int reuse = 1;
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(const char *)&reuse,sizeof(int))<0) {
		return bail("setsockopt(2)");
	}
	#ifdef SO_REUSEPORT
	if(setsockopt(s,SOL_SOCKET,SO_REUSEPORT,(const char *)&reuse,sizeof(int))<0)
		bail("setsockopt(2)");
	#endif
	
	memset(&adr_srvr,0,sizeof(adr_srvr));
	adr_srvr.sin_family = AF_INET;
	adr_srvr.sin_port = htons(atoi(srvr_port1));
	if ( strcmp(srvr_addr,"*") != 0 )
	{
		adr_srvr.sin_addr.s_addr = inet_addr(srvr_addr);
		if ( adr_srvr.sin_addr.s_addr == INADDR_NONE )
        {
			close(s);
            return bail("bad address.");
        }
	}
	else
	{
		adr_srvr.sin_addr.s_addr = INADDR_ANY ;
	}
	len_inet = sizeof(adr_srvr);
	
	while(true) {
    	z = bind(s,(struct sockaddr *)&adr_srvr,len_inet);
    	if(z==-1) {
    	    cout<<"Bail"<<endl;continue;
    	}
    	else {
    	    break;
	    }
	}
	//if ( z == -1 ) {
	//	close(s);
	//	return bail("bind(2)");
	//}

	z = listen(s,10);
	if ( z == -1 )
    {
		close(s);
		return bail("listen(2)");
    }
	//cout<<"MAKING CONNECTIONS WITH THE CLIENT"<<endl;
    len_inet = sizeof(adr_clnt);
    c = accept(s,(struct sockaddr *)&adr_clnt, &len_inet);
    if ( c == -1 )
    {
		close(s);
        return bail("accept(2)");
    } else
    {
        //cout<<"CONNECTION ACCEPTED."<<endl;
    }
    /* BRAIN OF THE PROGRAM*/
    char arr[1000];
    //cout<<"REQUEST ACCEPTED."<<endl;
    //cout<<"UPLOADING A FILE..."<<endl;
	int sent = 0;
	int size = 0;
	while(infile)
	{
   	 	infile.read((char *)&arr,sizeof(arr));
		size += infile.gcount();
	}
	char a[10];
	sprintf(a, "%d", size);
	string dest = string(10 - strlen(a), '0').append(a);
	char send_len[10];
	strcpy(send_len, dest.c_str());
	// //cout<<":"<<send_len<<":"<<endl; //test
	z=send(c,send_len,10,0);
	if(z==-1)
    {
		close(c);
		close(s);
        return bail("write(2)");
    }
	infile.close();

	usleep(1*1000);
	close(s);
	close(c);

	infile.open(filename,ios::in|ios::binary);
	//int z;
 	//char srvr_addr[20]; /*CORRECTION*/
 	char srvr_port2[6];
	strcpy(srvr_port2, PORT2);
	//struct sockaddr_in adr_srvr;	/* AF_INET */
	//struct sockaddr_in adr_clnt;	/* AF_INET */
 	//socklen_t len_inet; 			/* length */
 	//int s; 				/* Socket */
 	//int c; 				/* Client socket */

	if(ip_address!=NULL)
    	strcpy(srvr_addr,ip_address);
    else
     	strcpy(srvr_addr,"127.0.0.1");
	//cout<<"trying to open socket"<<endl;
	
	
	
	s = socket(AF_INET,SOCK_STREAM,0);
	if ( s == -1 ) {
		//return bail("socket()");
		bail("socket()");
	}
	int reuse2 = 1;
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(const char *)&reuse2,sizeof(int))<0) {
		return bail("setsockopt(2)");
	}
	#ifdef SO_REUSEPORT
	if(setsockopt(s,SOL_SOCKET,SO_REUSEPORT,(const char *)&reuse,sizeof(int))<0)
		bail("setsockopt(2)");
	#endif
	
	memset(&adr_srvr,0,sizeof(adr_srvr));
	adr_srvr.sin_family = AF_INET;
	adr_srvr.sin_port = htons(atoi(srvr_port2));
	if ( strcmp(srvr_addr,"*") != 0 )
	{
		adr_srvr.sin_addr.s_addr = inet_addr(srvr_addr);
		if ( adr_srvr.sin_addr.s_addr == INADDR_NONE )
        {
			close(s);
            return bail("bad address.");
        }
	} else
	{
		adr_srvr.sin_addr.s_addr = INADDR_ANY ;
	}
	len_inet = sizeof(adr_srvr);
	
	while(true) {
    	z = bind(s,(struct sockaddr *)&adr_srvr,len_inet);
    	if(z==-1) {
    	    cout<<"Bail"<<endl;continue;
    	}
    	else {
    	    break;
	    }
	}
	//if ( z == -1 ) {
	//	close(s);
	//	return bail("bind(2)");
	//}

	z = listen(s,10);
	if ( z == -1 )
    {
		close(s);
		return bail("listen(2)");
    }

	//cout<<"MAKING CONNECTIONS WITH THE CLIENT"<<endl;
    len_inet = sizeof(adr_clnt);
    c = accept(s,(struct sockaddr *)&adr_clnt, &len_inet);
    if ( c == -1 )
    {
		close(c);
		close(s);
        return bail("accept(2)");
    } else
    {
        //cout<<"CONNECTION ACCEPTED."<<endl;
    }
    while(infile)
	{
   	 	infile.read((char *)&arr,sizeof(arr));
		int charcount = infile.gcount();
		////cout<<charcount<<endl;
		//testfile.write((char *)&arr, charcount);
		sent += charcount;
		//cout<<charcount<<" "<<sent<<endl;  //test
		z=send(c,arr,charcount,0);
		if(z==-1)
        {
			close(c);
			close(s);
            return bail("write(2)");
        }
	}
	usleep(1*1000);
    close(c);
	////cout<<"sent: "<<sent<<endl;
    //cout<<"UPLOADING COMPLETE."<<endl;
    //cout<<"Closing socket..."<<endl;
    infile.close();
    close(s);
	return 0;
}

int obj_receive(const char PORT1[], const char PORT2[], char filename[], char ip_address[])
{
	fstream outfile;
    //system("[ -e "+string(filename)+" ] && rm "+filename); //256 if not exists
	// system("rm "+string(filename));
   	outfile.open(filename,ios::out|ios::binary);
 	int z;
 	char srvr_addr[20];
 	char srvr_port1[6];
	strcpy(srvr_port1, PORT1);
 	struct sockaddr_in adr_srvr;	/* AF_INET */
 	socklen_t len_inet; 			/* length */
 	int s; 				/* Socket */
	if(ip_address!=NULL)
    	strcpy(srvr_addr,ip_address);
    else
     	strcpy(srvr_addr,"127.0.0.1");
     	
    s = socket(AF_INET,SOCK_STREAM,0);
	if ( s == -1 ) {
		//return bail("socket()");
		bail("socket()");
	}
	int reuse = 1;
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(const char *)&reuse,sizeof(int))<0) {
		return bail("setsockopt(2)");
	}
	#ifdef SO_REUSEPORT
	if(setsockopt(s,SOL_SOCKET,SO_REUSEPORT,(const char *)&reuse,sizeof(int))<0)
		bail("setsockopt(2)");
	#endif
	
 	memset(&adr_srvr,0,sizeof(adr_srvr));
 	adr_srvr.sin_family = AF_INET;
 	adr_srvr.sin_port = htons(atoi(srvr_port1));
 	adr_srvr.sin_addr.s_addr = inet_addr(srvr_addr);
 	if ( adr_srvr.sin_addr.s_addr == INADDR_NONE )
    {
		close(s);
		return bail("bad address.");
    }
	len_inet = sizeof(adr_srvr);
    //cout<<"MAKING CONNECTIONS WITH THE SERVER."<<endl;
 	z = connect(s,(struct sockaddr*)&adr_srvr,len_inet);
 	if ( z == -1 )
    {
		close(s);
		return bail("connect(2)");
    } else {
		//cout<<"CONNECTED TO SERVER"<<endl;
	}
	//cout<<"MAKING REQUEST FOR THE FILE."<<endl;
	//cout<<"DOWNLOADING A FILE..."<<endl;
 	/* BRAIN OF THE PROGRAM*/
	char data[30000];
 	char dbuf[10];
	usleep(1*1000);
	recv(s,data,10,0);
	strcpy(dbuf, string(data).substr(0,10).c_str());
	//cout<<sizeof(dbuf)<<":"<<dbuf<<":"<<endl;
	int r = atoi(dbuf);
	cout<<r<<endl;

	usleep(1*1000);
    close(s);
	
	strcpy(srvr_port1,PORT2);
	if(ip_address!=NULL)
    	strcpy(srvr_addr,ip_address);
    else
     	strcpy(srvr_addr,"127.0.0.1");
     	
    s = socket(AF_INET,SOCK_STREAM,0);
	if ( s == -1 ) {
		//return bail("socket()");
		bail("socket()");
	}
	int reuse2 = 1;
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(const char *)&reuse2,sizeof(int))<0) {
		return bail("setsockopt(2)");
	}
	#ifdef SO_REUSEPORT
	if(setsockopt(s,SOL_SOCKET,SO_REUSEPORT,(const char *)&reuse,sizeof(int))<0)
		bail("setsockopt(2)");
	#endif
	
 	memset(&adr_srvr,0,sizeof(adr_srvr));
 	adr_srvr.sin_family = AF_INET;
 	adr_srvr.sin_port = htons(atoi(srvr_port1));
 	adr_srvr.sin_addr.s_addr = inet_addr(srvr_addr);
 	if ( adr_srvr.sin_addr.s_addr == INADDR_NONE )
    {
		close(s);
		return bail("bad address.");
    }
	len_inet = sizeof(adr_srvr);
    //cout<<"MAKING CONNECTIONS WITH THE SERVER."<<endl;
 	z = connect(s,(struct sockaddr*)&adr_srvr,len_inet);
 	if ( z == -1 )
    {
		close(s);
		return bail("connect(2)");
    } else {
		//cout<<"CONNECTED TO SERVER"<<endl;
    }
	usleep(1*1000000);
	recv(s,data,r,0);
	outfile.write((char *)&data, r);
   	outfile.close();
   	//cout<<"CLOSING SOCKET..."<<endl;
	usleep(1*1000);
 	close(s);
 	
 	cout<<"Object file has been received"<<endl;
	system(string("chmod +rwx "+string(filename)).c_str());
	// //cout<<"Executing the object file....."<<endl;
	// //cout<<"Object file output:"<<endl;
	// system("./obj");
	// //cout<<"Finished execution"<<endl;
	return 0;
}
/*
int main() {
    //cout<<"send"<<endl;
    //cout<<send_command("0000", "DONE", "172.16.100.209", true);
    //string command = receive_command("9100");
    return 0;
}
*/
