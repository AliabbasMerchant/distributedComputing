#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdlib>
#include<string.h>
#include<vector>

// #include "dc/initial.h"
#include "dc/help.h"
#include "dc/networking.h"
using namespace std;

bool connected = false;
int args_received = 0;
int pc_no;
bool received_exec_file = false;
bool everything_done = false;
char server_ip[18];
char self_ip[18];
char exec_filename[] = "dc/obj";

char current_args[30];
char current_ans[25];

void connect(const char PORT[])
{
    string command = receive_command(PORT, server_ip);
    if(strcmp("CONNECT",string(command).substr(0,7).c_str())==0)
	{
		cout<<&command[7]<<endl;
        pc_no = atoi(&command[7]);
        connected = true;
	}
}
// void get_exec_file(char PORT1[], char PORT2[])
void get_exec_file(const char *PORT1, const char *PORT2)
{
    if(obj_receive(PORT1, PORT2, exec_filename, server_ip) == 0)
        received_exec_file = true;
}
bool listen_for_commands(const char PORT[])
{
    string command = receive_command(PORT, server_ip);
    if(strcmp("___", command.c_str())==0) {
        cout<<"Error listening for commands"<<endl;
        return false;
    } else if(strcmp("DONE", command.c_str())==0) {
		cout<<"RECEIVED DONE!!!!"<<endl;
		everything_done = true;
	} else // command is the argument to solve for
	{
		cout<<"RECEIVED: "<<command<<endl;
        args_received++; // should not excedd 98 // coz (maybe, max) 99 is for 'everything is done'
        strcpy(current_args, command.c_str());
	}
    return true;
}
bool solve(string arg) // bool to return for error
{
    // system("chmod +rwx dc/"+string(exec_filename));
    cout<<"./"<<string(exec_filename)<<" "<<arg<<endl;
    if(system("./"+string(exec_filename)+" "+arg) == 0) // user_template.cpp must return 0
    {
        return true;
    }
    else
    {
        return false;
    }
}
void send_ans(const char PORT[])
{
    bool sent=false;
    while(!sent) {
        if(send_command(PORT, current_ans, self_ip) == 0 ) {
            sent = true;
        }
    }
	cout<<"Sending answer. PORT="<<PORT<<" ans="<<current_ans<<endl;
}
void get_ans_from_file()
{
    std::fstream file("dc/dist_result.txt", std::ios::in);
    file.getline(current_ans, 25);
    file.close();
}
int main(int argc,char **argv)
{
    strcpy(server_ip,argv[1]);
    strcpy(self_ip,argv[2]);
    int i=1; // for rotation of last char of port number
    while(!connected)
    {
		//usleep(2*1000000);
		cout<<"PORT = "<<(string("1000")+its(i))<<endl;
        connect((string("1000")+its(i)).c_str());
        i++;
        if(i>9) i=1;
    }
	i=0;
	//usleep(3*1000000);
    while(!received_exec_file)
    {
        // no rotation of port number
        get_exec_file( (string("100")+its(pc_no)+string("0")).c_str(), (string("100")+its(pc_no)+string("1")).c_str() );
		//usleep(1*1000000);
    }
    //exit(0);
    while(!everything_done)
    {
        int arg_no = args_received;
        if(listen_for_commands( (string("1")+its(pc_no)+its(arg_no, 2)+string("0")).c_str() ))
        {
            if(everything_done)
            {
                break;
            }
            else // proper command
            {
                bool solved = false;
                while(!solved)
                {
                    if(solve( string(current_args) ))
                    {
                        solved = true;
                        get_ans_from_file();
                        send_ans( (string("1")+its(pc_no)+its(arg_no, 2)+string("1")).c_str() );
                    }
                    usleep(1 * 1000000);
                }
            }
        } else {
			//usleep(2*1000000);
		}
    }
    return 0;
}
