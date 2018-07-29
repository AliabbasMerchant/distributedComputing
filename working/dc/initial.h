#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdlib>
#include<string.h>
#include<string>
#include<vector>
#include <unistd.h>
#include<algorithm>
//#include <bits/stdc++.h>
#include <algorithm> 
#include <cctype>
#include <locale>
#include "create_files.h"
#include"help.h"
using namespace std;


//struct result_pair
//{
//    char args[30];
//    char ans[25];
//};

string create_files_and_run_user_number(char filename[100])
{
    char number_filename[100] = "";
    char template_filename[100] = "";
    int i;
    for(i=0; i<strlen(filename)-4; i++)
    {
        number_filename[i] = filename[i];
        template_filename[i] = filename[i];
    }
    strcat(number_filename, "_number.cpp");
    cout<<number_filename<<endl; //test
    strcat(template_filename, "_template.cpp");
    cout<<template_filename<<endl; //test

    generate_req_files(filename, number_filename, template_filename);

    string exec_file = "dc/dist_number";
    if(system("g++ " + string(number_filename) + " -o " + string(exec_file)) == 0)
	{   cout<<"The program has been successfully compiled!"<<endl;  //test
        system("chmod +rwx " + string(exec_file));
        if(system("./" + string(exec_file)) == 0)
        {   cout<<"The program " + exec_file + " has been executed!"<<endl;  //test
        }
        else
        {   cout<<"Sorry, an error has occurred while executing the 'number' source code."<<endl;
            exit(0);
        }
	}
	else
	{   cout<<"Sorry, an error has occurred while compiling the 'number' source code."<<endl;
        exit(0);
    }
    
    exec_file = "dc/dist_template";
    if(system("g++ " + string(template_filename) + " -o " + string(exec_file)) == 0)
	{
	    cout<<"The program has been successfully compiled!"<<endl;  //test
	}
	else
	{
        cout<<"Sorry, an error has occurred while compiling the template source code."<<endl;
        exit(0);
	}
    return exec_file;
}
int get_no_of_iterations()
{
    fstream listfile;
    listfile.open("dc/list.txt", ios::in);
    int number_of_iterations = 0;
    char c[100];
    while(!listfile.eof())
    {
        listfile.getline(c, 100);
        if (strcmp(c, "")!=0)
        {
            number_of_iterations++;
        }
    }
    listfile.close();
    return number_of_iterations;
}

/*
result_pair* note_args()
{
    char c[30];
    fstream listfile;
    listfile.open("list.txt", ios::in);
    result_pair res[number_of_iterations];
    int i = 0;
    while(!listfile.eof())
    {
        listfile.getline(c, 30);
        if (strcmp(c, "")!=0)
        {
            string s = string(c);
            trim(s);
            strcpy(res[i].args, s.c_str());
            //cout<<":"<<res[i].args<<":"<<endl; //test
            i++;
        }
    }
    listfile.close();
    return res;
}
*/

bool compile(char filename[100])
{
	if(system("g++ " + string(filename) + " -o dist_user_prog") == 0)
	{
	    cout<<"The program has been successfully compiled!"<<endl;
	    system("rm dist_user_prog");
        return true;
	}
	else
	{
        cout<<"Compilation of the source code has failed!"<<endl;
        cout<<"Please recheck your program for any errors."<<endl;
        return false;
	}
}
