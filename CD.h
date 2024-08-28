#ifndef CD_H
#define CD_H

#include<unistd.h>
#include<iostream>

using namespace std;

int CD(string path)
{
    return chdir(&path[0]);
}

#endif