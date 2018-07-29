#include<iostream>
#include<algorithm>
#include <bits/stdc++.h>
#include <algorithm> 
#include <cctype>
#include <locale>
using namespace std;
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
string get_lcv(string for_loop)
{
    // clean up:
    // strip
    // remove comments
    // cout<<for_loop<<endl;
    int first_bracket=0, semicolon=0;
    if (for_loop.find("(") != string::npos)
    {   // cout << "( found at " << for_loop.find("(") << endl;
        first_bracket = for_loop.find("(");
    }
    else
    {
        return "";
    }

    if (for_loop.find(";") != string::npos)
    {   // cout << "; found at " << for_loop.find(";") << endl;
        semicolon = for_loop.find(";");
    }
    else
    {
        return "";
    }
 
    //  substr(a, b) function returns a substring of b length
    //  starting from index a
    string initialization = for_loop.substr(first_bracket+1, semicolon-first_bracket-1);
    // cout<<":"<<initialization<<":"<<endl<<endl; //int i = 0

    int equal_to=0;
    if (initialization.find("=") != string::npos)
    {   // cout << "= found at " << initialization.find("=") << endl;
        equal_to = initialization.find("=");
    }
    else
    {
        // TODO ALIABBAS
        return "";
    }
    string removed_equal_to = initialization.substr(0, equal_to);
    trim(removed_equal_to);
    // cout<<":"<<removed_equal_to<<":"<<endl; //int i = 0
    
    reverse(removed_equal_to.begin(), removed_equal_to.end());
    // cout<<":"<<removed_equal_to<<":"<<endl;
    
    string desired_rev = "";
    for(int i =0;i<removed_equal_to.size();i++)
    {
        if(removed_equal_to[i]==' ')
        {
            bool valid = false;
            for(int ii=0; ii<=i; ii++)
            {
                char ch = removed_equal_to[ii];
                if((ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A'))
                {
                    valid = true;
                    break;
                }
            }
            // if there is at least 1 alphabet in desired_rev
            if(valid) { break; }
            else { desired_rev.push_back(removed_equal_to[i]); }
        }
        else
        {
            desired_rev.push_back(removed_equal_to[i]);
        }
    }
    reverse(desired_rev.begin(), desired_rev.end());
    return desired_rev;
}