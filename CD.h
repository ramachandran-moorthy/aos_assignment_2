#ifndef CD_H
#define CD_H

#include<unistd.h>
#include<iostream>

using namespace std;

void CD(string path, string &currentdir, string &workingdir, string &prevdir)
{
    int ret;
    if(path=="~")
        ret = chdir(&workingdir[0]);
    else if(path=="-")
    {
        if(prevdir=="")
        {
            string str = "Invalid Input: No previous directory record exists.\n";
            write(1, &str[0], str.length());
            return;
        }
        ret = chdir(&prevdir[0]);
    }
    else
        ret = chdir(&path[0]);
    if(ret!=0)
    {
        perror("Invalid input");
    }
    else
        prevdir = currentdir;
}

#endif