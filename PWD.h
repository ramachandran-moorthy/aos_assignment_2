#ifndef PWD_H
#define PWD_H

#include<unistd.h>
#include<iostream>

using namespace std;

void getPWD()
{
    char buffer[100];
    string str;
    getcwd(buffer, 100);
    str = buffer;
    str+="\n";
    write(1, &str[0], str.length());
}

#endif