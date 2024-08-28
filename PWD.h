#ifndef PWD_H
#define PWD_H

#include<unistd.h>
#include<iostream>

using namespace std;

string getPWD()
{
    char buffer[100];
    getcwd(buffer, 100);
    return buffer;
}

#endif