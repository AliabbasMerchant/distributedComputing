#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdlib>
#include<string.h>
#include<string>
#include<thread>
#include<vector>
#include <unistd.h>

#include "dc/initial.h"
#include "dc/networking.h"
// #define SELF_IP "172.161.16.41"
using namespace std;

char SELF_IP[18];
bool done_listening_for_clients = false;
int no_of_pcs;
bool uploaded_obj_file = false;
int number_of_iterations;
string obj_filename;
int no_of_clients = 0;

struct result_pair
{
    char args[30];
    char ans[25];
};
struct pc
{
    char ip[25];
    int status=1; //0-occupied, 1-free
    int sent_args=0;
};

vector<pc> pcs; //client_details;
//result_pair *results;
vector<result_pair> results;

void client_connect(bool extra_arg, string PORT, string command)
{
    cout<<"on client_connect thread"<<endl;
	connect:
	string s = send_command(PORT.c_str(), command.c_str() ,SELF_IP, true);
    if(strcmp(s.c_str(), string("ERROR").c_str()) == 0)
    {
    	//usleep(1*1000000);
        goto connect;
    } else
    {
        pc my_pc;
        strcpy(my_pc.ip, s.c_str());
        my_pc.status = 1;
        my_pc.sent_args = 0;
        pcs.push_back(my_pc);
        cout<<"Client has connected!!!!!!!!!!!!!!!!!!"<<endl;
        cout<<"Index= "<<pcs.size()-1<<endl;
        cout<<"Status= "<<pcs[pcs.size()-1].status<<endl;
        cout<<"IP= "<<pcs[pcs.size()-1].ip<<endl;
        cout<<"Sent_args= "<<pcs[pcs.size()-1].sent_args<<endl;
        // pcs.push_back(pc());
        // pcs[pcs.size() - 1].sent_args = 0;
        // pcs[pcs.size() - 1].status = 1;
        // strcpy(pcs[pcs.size() - 1].ip, s.c_str());
    }
}

void note_args()
{
    char c[30];
    fstream listfile;
    listfile.open("dc/list.txt", ios::in);
    //result_pair res[number_of_iterations];
    int i = 0;
    while(!listfile.eof())
    {
        listfile.getline(c, 30);
        if (strcmp(c, "")!=0)
        {
			//cout<<"------------------------------------------------------------i = "<<i<<", args = "<<c<<endl;
			result_pair my_pair;
	    	strcpy(my_pair.args, c);
		    results.push_back(my_pair);
            i++;
        }
    }
    listfile.close();
//	for(int k=0;k<i;k++)
//	{
//		strcpy(results[k].args,res[k].args);
//		strcpy(results[k].ans,res[k].ans);
//	}
	//results = res;
}

bool _solve(char current_args[]) // bool to return for error
{
    // system("chmod +rwx " +string(obj_filename));
	cout<<"command = "<<("./"+string(obj_filename)+" "+string(current_args))<<endl;
    if(system("./"+string(obj_filename)+" "+string(current_args)) == 0) // user_template.cpp must return 0
    {
        return true;
    }
    else
    {
        return false;
    }
}
string get_ans_from_file()
{
    char current_ans[25];
    std::fstream file("dc/dist_result.txt", std::ios::in);
    file.getline(current_ans, 25);
    file.close();
    return string(current_ans);
}
void solve(int iteration_no, int pc_no)
{
	cout<<"solving:  iteration_no="<<iteration_no<<" pc_no="<<pc_no<<endl;  //test
    if(pc_no == 0) // the master pc itself
    {
		bool solved = false;
        while(!solved)
        {
            if(_solve(results[iteration_no].args))
            {
                solved = true;
				cout<<"solved = true;"<<endl; //test
                string received_ans = get_ans_from_file();
				cout<<"received_ans = "<<received_ans<<endl;
                strcpy(results[iteration_no].ans, received_ans.c_str());
            }
        }
    }
    else
    {
        int sent = send_command( (string("1")+its(pc_no)+its(pcs[pc_no].sent_args, 2) + string("0")).c_str(), results[iteration_no].args, SELF_IP);
        while(sent != 0)
        {
			//usleep(1*1000000);
            sent = send_command( (string("1")+its(pc_no)+its(pcs[pc_no].sent_args, 2) + string("0")).c_str(), results[iteration_no].args, SELF_IP);
        }
		cout<<"Sent to pc. PORT="<<(string("1")+its(pc_no)+its(pcs[pc_no].sent_args, 2) + string("0"))<<endl;
        pcs[pc_no].sent_args++;
        //usleep(2 * 1000000);
        //get ans from client

        bool got_answer = false;
        string answer;
        while(!got_answer) {
            answer = receive_command((string("1")+its(pc_no)+its(pcs[pc_no].sent_args-1, 2) + string("1")).c_str(), pcs[pc_no].ip);
            if(strcmp("___", answer.c_str())==0) {
				//usleep(1*1000000);
                // cout<<"Error listening for the answer"<<endl;
            } else // command is the answer to solve for
            {
				cout<<"ANSWER = "<<answer<<endl;
                got_answer = true;
                strcpy(results[iteration_no].ans, answer.c_str());
            }
        }
    }
	cout<<"------------pc_no = "<<pc_no<<", Arg = "<<results[iteration_no].args<<
			", Answer = "<<results[iteration_no].ans<<", iteration_no = "<<iteration_no<<endl; //test
    // then join the current thread // I dont think its needed
    pcs[iteration_no].status = 1; //its now free
}
void print_results()
{
    fstream resultfile("dc_results.txt", ios::out);
    for(int i=0;i<number_of_iterations;i++)
    {
        resultfile<<i<<") "<<results[i].args<<" "<<results[i].ans<<endl;
    }
    resultfile.close();
}
void send_file(const char PORT1[], const char PORT2[], const char filename[])
{
	cout<<"PORT1 = "<<PORT1<<endl;
	cout<<"PORT2 = "<<PORT2<<endl;
    int sent = obj_send(PORT1, PORT2, filename, SELF_IP);
    while(sent != 0)
    {
        sent = obj_send(PORT1, PORT2, filename, SELF_IP);
    }
	cout<<"File sent"<<endl;
}
void everything_done(int pc_index)
{
    int sent = send_command( (string("1")+its(pc_index)+its(pcs[pc_index].sent_args, 2)+string("0")).c_str(), string("DONE").c_str(), SELF_IP);
    while(sent != 0)
    {
        sent = send_command( (string("1")+its(pc_index)+its(pcs[pc_index].sent_args, 2)+string("0")).c_str(), string("DONE").c_str(), SELF_IP);
    }
    
}
int main(int argc, char * argv[4])
{
    strcpy(SELF_IP, argv[2]);
    cout<<SELF_IP<<endl; //test
    no_of_clients = atoi(argv[3]);
    cout<<"no_of_clients"<<no_of_clients<<endl; //test
	pc my_pc;
    strcpy(my_pc.ip, SELF_IP);
    my_pc.status = 1;
    my_pc.sent_args = 0;
    pcs.push_back(my_pc);
	thread thrs[no_of_clients];
	for(int i=1;i<=no_of_clients;i++)
    {
		
//        if(!done_listening_for_clients)
//        {
            cout<<i<<endl;
			thrs[i-1] = thread(client_connect, true, (string("1000") + its(i)), (string("CONNECT")+its(i)));
			usleep(5*1000000);
//        } else
//        {
//            break;
//        }
    }

//    thread t1 = thread(client_connect, true);
	if(! compile(argv[1]))
    {
        done_listening_for_clients = true;
		for(int i=0;i<no_of_clients;i++)
		{
			thrs[i].join();
		}
        return 0;
    }

    system("cp "+string(argv[1])+" dc/"+string(argv[1]));
    char filename[100];
    strcpy(filename, (string("dc/")+string(argv[1])).c_str());
    obj_filename = create_files_and_run_user_number(filename); //argv[1] is the filename (eg "user.cpp")
    cout<<"Initital work done!"<<endl;
    done_listening_for_clients = true;
    cout<<"done_listening_for_clients = true"<<endl<<endl<<endl<<endl;
    for(int i=0;i<no_of_clients;i++)
	{
		thrs[i].join();
	}
	no_of_pcs = pcs.size();
    cout<<"NO of PCs = "<<no_of_pcs<<endl;
	cout<<pcs[0].ip<<endl;
	cout<<pcs[1].ip<<endl;
    //exit(0); //test



    thread thr[no_of_pcs];
    for(int i=1; i<no_of_pcs; i++) // 0 is the master. It already has the exec file.
    {
		cout<<"PORT = "<<(string("100")+its(i)+string("0"))<<endl;
        thr[i] = thread(send_file, (string("100")+its(i)+string("0")).c_str(), (string("100")+its(i)+string("1")).c_str(), obj_filename.c_str());
    }
    number_of_iterations = get_no_of_iterations();
    cout<<"number_of_iterations = "<<number_of_iterations<<endl; //test
    note_args();
	cout<<"Args noted"<<endl;
	for(int i = 0; i<number_of_iterations; i++) {
		//cout<<"------------------------------------------------------------i = "<<i<<", args = "<<results[i].args<<endl;
	}
    for(int i=1; i<no_of_pcs; i++)
    {
        thr[i].join();
    }
    //exit(0); //test


	thread threads[number_of_iterations];
    for(int i=0;i<number_of_iterations; i++)
	//for(int i=0;i<2; i++)
    {
        cout<<"Args = "<<results[i].args<<endl; //test
        //send args to clients
        bool assigned_a_pc = false;
        for(int j=0;j<no_of_pcs;j++) // 0 is the master
        {
			if(!assigned_a_pc) {
		        if(pcs[j].status == 1) // free //else it is busy
		        {
		            assigned_a_pc = true;
					cout<<"assigned_a_pc = true "<<j<<endl; //test
		            pcs[j].status = 0; //its now busy
		            threads[i] = thread(solve, i, j);
		        }
			}
			if(!assigned_a_pc && j==(no_of_pcs-1)) {
				//usleep(1 * 1000000);
				j=0;
			}
        }
    }
    for(int i=0;i<number_of_iterations;i++)
    {
        threads[i].join();
    }
	//waitforcommand:
	//cout<<"Press c: "<<endl;
	//char character;
	//cin>>character;
	//if(character!='c') goto waitforcommand;
    for(int i=1;i<no_of_pcs;i++)
    {
        thr[i] = thread(everything_done, i);
    }
	//exit(0);
    print_results();
    for(int i=1;i<no_of_pcs;i++)
    {
        thr[i].join();
    }
    return 0;
}
