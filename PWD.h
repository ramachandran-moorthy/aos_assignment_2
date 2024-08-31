#ifndef PWD_H
#define PWD_H

#include<unistd.h>
#include<iostream>

using namespace std;

//this function prints the pwd based on the home directory
//this function return the absolute path of the current working directory
string getPWD()
{
    string ret="";
    char buffer[100];
    string str;
    getcwd(buffer, 100);
    str = buffer;
    return str;
}
string getDisplayPath(string workingDir)
{
    string str = getPWD();
    string ret="";
    if(str==workingDir)
        ret =  "~";
    else if(str.length()>workingDir.length() && str.substr(0,workingDir.length())==workingDir)
        ret = "~"+str.substr(workingDir.length());
    else
        ret = "~"+str;
    return ret;
}

#endif