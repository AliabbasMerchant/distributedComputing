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

using namespace std;

/*
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}
*/

int system(string command)
{
    char char_array[command.length()+1]; 
    strcpy(char_array, command.c_str()); 
    return system(char_array);
}
string its(int i, int digits=1)
{
    char c[digits + 1];
	sprintf(c, "%d", i);
	return string(digits - strlen(c), '0').append(c);
}
// int sti(string s)
// {
//     return 0;
// }
