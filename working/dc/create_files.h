#include<iostream>
#include<algorithm>
#include<fstream>
#include<vector>
#include<cstring>
#include<cstdlib>
#include "lcv.h"
using namespace std;

ifstream infile;
char user_file[100], temp_file[100], num_file[100];
int p_start=0, p_main_args, p_loop_start, p_loop_end, p_call_start, p_call_end;
char lcv[100], p_cif_name[100], loop_statement[200];
int cif_call_line_start;
int cif_call_line_end;


int zero_pointer;    ////pointer 0
vector <int> cif_return_start;   ////pointer 2
vector <int> cif_return_end;   ////pointer 12
vector <string> cif_function_name;
int main_args;        ////pointer 5
vector <int> loop_start;    ////pointer 3
vector <int> loop_end;    ////pointer 4
vector <int> cif_call_start;  ////pointer 7
vector <int> cif_call_end;   ////pointer 8

vector <int> searchcomment();
void searchcif();
void searchloop();
int searchmain();


vector<int>  searchcomment()
{
	infile.open(user_file, ios::ate);
	infile.seekg(0, ios::beg);
	vector <int> in;
	vector <int> ::iterator i;
	vector <char> ch;
	vector <char> ::iterator j;
	char c = 'a';
	char cvec1 = 'a', cvec2 = 'a', cvec3 = 'a';
	int inp1, inp2, inp3, pointpos = 1;
	for ((infile.get(c)); (infile.eof() == 0); (infile.get(c)), ++pointpos)
	{
		if (c == '/')
		{
			inp1 = pointpos;
			cvec1 = c;
			infile.get(c);
			++pointpos;
			if (c == '/')
			{
				inp2 = pointpos;
				cvec2 = c;
				in.push_back(inp1); in.push_back(inp2);
				ch.push_back(cvec1); ch.push_back(cvec2);
				infile.get(c);
				++pointpos;
				for (; c != '\n'; (infile.get(c)), ++pointpos)
				{
					inp3 = pointpos;
					cvec3 = c;
					in.push_back(inp3);
					ch.push_back(cvec3);
				}
				if (c == '\n')
				{
					inp3 = pointpos;
					cvec3 = c;
					in.push_back(inp3);
					ch.push_back(cvec3);
				}
			}
			else if (c == '*')
			{
				inp2 = pointpos;
				cvec2 = c;
				in.push_back(inp1); in.push_back(inp2);
				ch.push_back(cvec1); ch.push_back(cvec2);
				int flag = 1;
				int inp4;
				char cvec4;
				while (flag)
				{
					infile.get(c);
					++pointpos;
					inp3 = pointpos;
					cvec3 = c;
					in.push_back(inp3);
					ch.push_back(cvec3);
					if (c == '*')
					{
						infile.get(c);
						++pointpos;
						inp4 = pointpos;
						cvec4 = c;
						in.push_back(inp4);
						ch.push_back(cvec4);
						if (c == '/')
						{
							flag = 0;
						}
					}

				}

			}
		}
	}

	infile.close();
	return (in);
}

void searchcif()
{
	infile.open(user_file, ios::ate);
	infile.seekg(0, ios::beg);
	vector <int> in;
	vector <int> ::iterator i;
	vector <char> y;
	vector <char> ::iterator z;
	vector <string> ::iterator u;


	char char_arr[500];
	char cif_name[100];
	char c = 'a';
	int nxtline = 0;
	int chars = 0;
	int pointpos = 0;
	int pars = 0, pare = 0;
	int x = 0;
	int l = 0;
	bool flag = true;
	char q;
	for (infile.getline(char_arr, 500, '\n'); (infile.eof() == 0); infile.getline(char_arr, 500, '\n'))
	{
		nxtline++;
		chars = chars + strlen(char_arr);
		if ((strcmp(string(char_arr).substr(0, 10).c_str(), "// __CIF__")) == 0)
		{
			pointpos = chars + nxtline;

			in.push_back(pointpos);
			++x;
		}
	}
	for (i = in.begin(); i != in.end(); i++)
	{
		//std::cout << *i << endl;

	}



	infile.close();


	infile.open(user_file, ios::ate);
	infile.seekg(0, ios::beg);
	char ch = 'a';
	char a[500];
	char b[500];
	string str;

	std::reverse(begin(in), end(in));

	int sentinel_for = 0;

	for (int p = 0; p<x; ++p)
	{
		pointpos = in.back();
		in.pop_back();
		for (int k = sentinel_for; k < pointpos; k++)
		{
			infile.get(ch);
		}

		infile.getline(a, 500, '\n');
		chars = strlen(a);                        
		str = a;
		
		int indexstart = str.find_first_of('(');
		string print;
		print = str.substr(0, indexstart);
		strcpy(b, print.c_str());
		for (l = 0; l < indexstart; l++)
		{
			y.push_back(b[l]);
		}
		q = y.back();
		while (flag)
		{
			q = y.back();
			if (q == ' ')
			{
				y.pop_back();

			}
			else
			{
				flag = false;
				break;
			}
			
		}
		for (z = y.begin(), l = 0; z != y.end(); z++, l++)
		{
			//cout<< *z;
		}
		for (l = 0; l < 100; l++)
		{
			//	cout << b[l];
		}
		strcpy(b, print.c_str());
		
		//indexstart = print.find_last_of(' ' || '::');////////********action needed 1 ***********///////////
		indexstart = string::npos;
		int temp_index1 = 0;
		int temp_index2 = 0;

		temp_index1 = print.find_last_of(' ');
		temp_index2 = print.find_last_of('::');
		if ((temp_index1 != string::npos) && (temp_index2 != string::npos))
		{
			if (temp_index1 < temp_index2)
			{
				indexstart = temp_index2;
			}
			else
			{
				indexstart = temp_index1;
			}
		}
		else
		{
			if (temp_index1 != string::npos)
			{
				indexstart = temp_index1;
			}
			if (temp_index2 != string::npos)
			{
				indexstart = temp_index2;
			}
		}

		str = print.substr(indexstart + 1);
		cif_function_name.push_back(str);
		pointpos += chars + 1;
		int bracketstart = 0, bracketend = 0;
		int flag = 1;
		while (flag)
		{
			infile.get(c);
			++pointpos;
			if (c == '{')
			{
				flag = 0;
				bracketstart = pointpos;
			}
		}
		int bracket = 1;
		int return1 = 0, return2 = 0;
		while (bracket != 0)
		{
			infile.get(c);
			++pointpos;
			if (c == '{')
			{
				++bracket;
			}

			int sentinel = 0;
			int h = 0, k = 0;
			char z[15] = "return";
			h = strlen(z);

			while (bracket != 0)     
			{
				infile.get(c);
				++pointpos;
				sentinel = pointpos;
				while (c == z[k] && sentinel == pointpos)
				{                                                     ////////////attention needed/////////////
																	  //std::cout << c << endl;
																	  //std::cout << pointpos << endl;
					if ((h - 1) == k)
					{
						infile.get(c);
						++pointpos;

						if (c == ' ')
						{
							cif_return_end.push_back(pointpos - 1);
							cif_return_start.push_back(pointpos - 6);
						}

					}
					++k;
					infile.get(c);
					++pointpos;
					++sentinel;
					if (c == '}')
					{
						--bracket;
					}
					if (bracket == 0)
						break;
				}
				sentinel = 0;
				k = 0;
				if (c == '}')
				{
					--bracket;
				}
			}

		}
		sentinel_for = pointpos;
	}
	infile.close();
	///////////////////////////////////////////////////////////////////
	cout << "in searchcif";
	cout << endl << "::printing cif_return_start::" << endl;
	for (i = cif_return_start.begin(); i != cif_return_start.end(); i++)
	{
		std::cout << *i << endl;

	}
	cout << endl << "::printing cif_return_end::" << endl;
	for (i = cif_return_end.begin(); i != cif_return_end.end(); i++)
	{
		std::cout << *i << endl;

	}
	cout << endl << "::printing cif_function_name::" << endl;
    strcpy(p_cif_name, cif_function_name[0].c_str());
	
}

int searchmain()
{
	infile.open(user_file, ios::ate);
	infile.seekg(0, ios::beg);
	char main = 'a';
	int flag = 1;
	int pointpos = 0;
	while (flag)
	{
		infile.get(main);
		pointpos++;
		if (main == 'm')
		{
			infile.get(main);
			pointpos++;
			if (main == 'a')
			{
				infile.get(main);
				pointpos++;
				if (main == 'i')
				{
					infile.get(main);
					pointpos++;
					if (main == 'n')
					{
						while (flag)
						{
							infile.get(main);
							pointpos++;
							if (main == '(')             /////////////////////////////////attention needed///////////////
								flag = 0;
						}
					}
				}
			}
		}

	}
	std::cout << "--------------------------main_args = "<<pointpos<<endl;
	main_args = pointpos;
	return 0;
}

void searchloop()
{
	ifstream infile;
	infile.open(user_file, ios::ate);
	infile.seekg(0, ios::beg);
	vector <int> in1;
	vector <int> ::iterator i;
	vector <int> in2;
	vector <int> ::iterator j;
	char arr[500];
	char c = 'a';
	int nxtline = 0;
	int chars = 0;
	int pointpos = 0;
	int k = 0;
	int sentinel = 0;
	int x = 0;
	bool flag = true;
	int for_rb = 0;

	for (infile.getline(arr, 500, '\n'); (infile.eof() == 0); infile.getline(arr, 500, '\n'))
	{
		nxtline++;
		chars = chars + strlen(arr);
		if ((strcmp(string(arr).substr(0, 11).c_str(), "// __LOOP__")) == 0)
		{
			pointpos = chars + nxtline;
			loop_start.push_back(pointpos);
			cout<<"LOOP START "<<pointpos<<endl;
			in1.push_back(pointpos);
			++x;
		}
	}
	for (i = loop_start.begin(); i != loop_start.end(); i++)
	{
		//std::cout << *i << endl;
	}
	
	infile.close();

	infile.open(user_file, ios::ate);
	infile.seekg(0, ios::beg);

	
	std::reverse(begin(in1), end(in1));
	int sentinel_for = 0;

	int pointer_begcif = 0;
	int pointer_argcif = 0;
	int entry_rb = 0;
	int start_rb = 0;
	int end_rb = 0;
	int count_rb = 0;

	std::reverse(begin(cif_function_name), end(cif_function_name));
	string temp;
	char a[150];
	for (int p = 0; p < x; ++p)
	{
		pointpos = in1.back();
		in1.pop_back();                                                     /////////////////////////////ATTENTION NEEDED////////////////////
		for (int t = 0; t < pointpos; t++)
		{
			infile.get(c);
		}

		/////////////////////////////////////////////////////////
		while (flag)
		{
			infile.get(c);
			++pointpos;
			if (c == '(')
			{
				++for_rb;
				while (for_rb != 0)
				{
					infile.get(c);
					++pointpos;
					if (c == '(')
					{
						++for_rb;
					}
					if (c == ')')
					{
						--for_rb;
						if (for_rb == 0)
						{
							flag = false;
							loop_end.push_back(pointpos);
						}
					}
				}

			}
		}////////////////////////////////////////////////////////////////////////////////
		flag = true;
		for_rb = 0;

		int h = 0;


		temp = cif_function_name.back();
		cif_function_name.pop_back();

		strcpy(a, temp.c_str());
		//char a[150] = "func";                               //////////////attention needed 4 ////////////////
		h = strlen(a);
		while ((infile.eof() == 0))
		{
			infile.get(c);
			++pointpos;
			sentinel = pointpos;
			while (c == a[k] && sentinel == pointpos)
			{
				if (((h - 1) == k) && c == a[(h - 1)])
				{
					infile.get(c);
					++pointpos;
					if (c == ' ')
					{
						pointer_begcif = sentinel - h;
						cif_call_start.push_back(pointer_begcif);
						break;
					}
					else if (c == '(')
					{
						++entry_rb;
						pointer_begcif = sentinel - h;
						cif_call_start.push_back(pointer_begcif);
						break;
					}
					else

						cout << "NOT FOUND" << endl;
				}
				++k;
				infile.get(c);
				++pointpos;
				++sentinel;
			}
			sentinel = 0;
			k = 0;

			if (pointer_begcif != 0)
			{
				infile.get(c);
				++pointpos;

				if (c == '(' || entry_rb == 1)
				{
					start_rb = pointpos;
					++count_rb;
					while (count_rb != 0)
					{
						infile.get(c);
						++pointpos;
						if (c == '(')
						{
							start_rb = pointpos;
							++count_rb;
						}
						if (c == ')')
						{
							end_rb = pointpos;
							--count_rb;
						}
					}
					if (count_rb == 0)
					{
						cif_call_end.push_back(pointpos);
						pointer_begcif = 0;
					}
					entry_rb = 0;
				}
			}
		}

		sentinel_for = pointpos;
		infile.close();
		infile.open(user_file, ios::ate);
		infile.seekg(0, ios::beg);
	}
	infile.close();
	//////////////////////////////////////////////////////////////////////////////////////////
	cout << "in searchloop";
	cout << endl << "::printing loop_start::" << endl;
	for (i = loop_start.begin(); i != loop_start.end(); i++)
	{
		std::cout << *i << endl;

	}
	cout << endl << "::printing loop_end::" << endl;
	for (i = loop_end.begin(); i != loop_end.end(); i++)
	{
		std::cout << *i << endl;

	}
	cout << endl << "::printing cif call start::" << endl;
	for (i = cif_call_start.begin(); i != cif_call_start.end(); i++)
	{
		std::cout << *i << endl;

	}
	cout << endl << "::printing cif call end::" << endl;
	for (i = cif_call_end.begin(); i != cif_call_end.end(); i++)
	{
		std::cout << *i << endl;

	}
}

int pos;
ifstream f;
ofstream f1, f2;
char c[100], copy[100];
int p[13], p_[13], p__[13], flag = 0, flagg = 0, i = 0, check = 0, checkk = 0, token = 0;


int generate_num_files() {
    char ch;
    int pos=-1;
    bool write=true, check_brace=false;
    int open_brace=0;
    // int p_start=0, p_main_args, p_loop_start, p_loop_end, p_call_start, p_call_end;
    // char lcv[100], p_cif_name[100], loop_statement[200];
    f.open(user_file);
    f2.open(num_file);
    while (f.eof() == 0)
    {
        f.get(ch);
        // cout<<ch;
        pos++;
        if(pos==p_start) {
            f2<<"#include<fstream>"<<endl<<"#include\"helper_funcs.h\""<<endl;
        } else if(pos==p_loop_start) {
            f2<<"\tstd::fstream file(\"dc/list.txt\", std::ios::out);"<<endl;
        } else if(pos==p_loop_end) {
            f2<<"\t{\n\t\tfile<<"<<lcv<<"<<std::endl;"	<<endl<<"\t}"<<endl<<"file.close();"<<endl<<"exit(0);"<<endl;
            f2<<"int "<<lcv<<"=0;"<<endl;
            // check_brace=true;
        }
        /*if(ch=='{' && check_brace && open_brace==0) {
            f1<<"{"<<endl<<"\tint "<<lcv<<" = atoi(argv[1]);"<<endl;
            write=false;
        }
        //////////////////////// Please do the thing and print everything except CIF call line
        if(ch=='}' && check_brace && open_brace==1) {
            f1<<"\tresult_store("<<p_cif_name<<"(a["<<lcv<<"]));"<<endl<<"}"<<endl<<"return 0;";
            // check_brace = false;
            write=false;
        }
        // if(check_brace) {
            // if(ch=='{') {
                // open_brace++;
            // }
            // if(ch=='}') {
                // open_brace--;
            // }
        // }
        */
        if(write) {
            f2<<ch;
        }
    }
    f.close();
    f2.close();
}
int generate_temp_files() {
    char ch;
    int pos=-1;
    bool write=true, check_brace=false;
    int open_brace=0;
    // int p_start=0, p_main_args, p_loop_start, p_loop_end, p_call_start, p_call_end;
    // char lcv[100], p_cif_name[100], loop_statement[200];
    f.open(user_file);
    f1.open(temp_file);
    while (f.eof() == 0)
    {
        f.get(ch);
        // cout<<ch;
        pos++;
        if(pos==p_start) {
            f1<<"#include<fstream>"<<endl<<"#include<stdlib.h>"<<endl<<"#include"<<"\"helper_funcs.h\""<<endl;
        } else if(pos==p_main_args) {
            f1<<"int argc, char * argv[2]";
        } else if(pos==p_loop_start) {
            write=false;
        } else if(pos==p_loop_end) {
            check_brace=true;
            write=true;
        }
        if(ch=='{' && check_brace && open_brace==0) {
            f1<<"{"<<endl<<"\tint "<<lcv<<" = atoi(argv[1]);"<<endl;
            write=false;
        }
        //////////////////////// Please do the thing and print everything except CIF call line
        if(ch=='}' && check_brace && open_brace==1) {
            f1<<"\tresult_store("<<p_cif_name<<"(a["<<lcv<<"]));"<<endl<<"}"<<endl<<"return 0;}";
            check_brace = false;
            write=false;
        }
        if(check_brace) {
            if(ch=='{') {
                open_brace++;
            }
            if(ch=='}') {
                open_brace--;
            }
        }
        if(write) {
            f1<<ch;
        }
    }
    f.close();
    f1.close();
}
int calc_pointers()
{
	//searchcomment();
	searchcif();
	searchmain();
	searchloop();

	/////////////////////////////////////////////////////////////////
	reverse(begin(cif_return_start), end(cif_return_start));
	reverse(begin(cif_return_end), end(cif_return_end));
	reverse(begin(cif_function_name), end(cif_function_name));
	reverse(begin(loop_start), end(loop_start));
	reverse(begin(loop_end), end(loop_end));
	reverse(begin(cif_call_start), end(cif_call_start));
	reverse(begin(cif_call_end), end(cif_call_end));
	////////////////////////////////////////////////////////////////////
	vector <int> dump;
	vector<int> ::iterator u;
	for (u = loop_start.begin(); u != loop_start.end(); u++)
	{
		dump.push_back(((*u) * 10) + 3);
	}
	for (u = loop_end.begin(); u != loop_end.end(); u++)
	{
		(*u) = (*u) + 3;
		dump.push_back(((*u) * 10) + 4);
	}
	dump.push_back((main_args * 10) + 5);
	std::sort(dump.begin(), dump.end(), std::greater<int>());
        cout<<"main_args = "<<main_args<<endl;
	
    
    cout<<"**********************************************************FOUND: "<<endl;
    p_main_args = main_args;
    p_loop_start = loop_start[0];
    p_loop_end = loop_end[0];
    p_call_start = cif_call_start[0];
    p_call_end = cif_call_end[0];
    cout<<"Calc loop"<<endl;
    f.open(user_file);
    int pos = 0;
    char ch;
    bool copy = false;
    while (f.eof() == 0)
    {
        f.get(ch);
        if(pos==p_loop_start) {
            copy=true;
        }
        if(pos==p_loop_end) {
            copy=false;
        }
        if(copy) {
            cout<<ch;
            char temp_arr[] = {ch};
            strcat(loop_statement, temp_arr);
        }
        pos++;
    }
    f.close();
    strcpy( lcv, get_lcv( (string) loop_statement).c_str() );

    string loop_s = string(loop_statement);
    trim(loop_s);
    strcpy(loop_statement, loop_s.c_str());

    cout<<"p_start ="<<p_start<<endl;
    cout<<"main_args = "<<p_main_args<<endl;
    cout<<"p_loop_start = "<<p_loop_start<<endl;
    cout<<"p_loop_end = "<<p_loop_end<<endl;
    cout<<"p_call_start = "<<p_call_start<<endl;
    cout<<"p_call_end = "<<p_call_end<<endl;
    cout<<"p_cif_name = "<<p_cif_name<<endl;
    cout<<"loop_statement = "<<loop_statement<<endl;
    cout<<"lcv = "<<lcv<<endl;
    

    return 0;
}
int generate_req_files(char user[], char num[], char temp[]) {
    strcpy(user_file, user);
    strcpy(num_file, num);
    strcpy(temp_file, temp);
    calc_pointers();
    cout<<endl<<endl<<"----------------------------------------------------------------------------------------------"<<endl;
	generate_temp_files();
    cout<<endl<<endl<<"----------------------------------------------------------------------------------------------"<<endl;
	generate_num_files();
    return 0;
}
