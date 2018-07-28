#include<fstream>
#include<iostream>
#include<cstdarg>
#include<string.h>
#include<cstdlib>
using namespace std;

template <class T>
T arg_store (int arg_len, char** args, T result) {
    std::fstream file("dc/dist_result.txt", std::ios::out);
    for(int i = 1;i<arg_len;i++)
    {   if(args[i]!="")
        {
            file<<args[i]<<std::endl;
   	    }
    }
    file<<result<<std::endl;
    file.close();
    return result;
}

template <class T>
T result_store (T result) {
	cout<<"test: "<<result<<endl;
    std::fstream file("dc/dist_result.txt", std::ios::out);
    file<<result<<std::endl;
    file.close();
    return result;
}
