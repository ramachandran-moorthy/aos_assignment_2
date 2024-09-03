#ifndef SEARCH_H
#define SEARCH_H

#include<unistd.h>
#include<iostream>
#include<dirent.h>
#include<sys/stat.h>
#include<string>

#include<regex>

using namespace std;

bool doSEARCH(string &str, string &currworkdir)
{
    DIR *directory = opendir(&currworkdir[0]);
    vector<string> directories, files;
    struct stat st;
    struct dirent *f;
    while((f = readdir(directory))!= NULL)
    {
        string name = f->d_name;
        if(name[0]=='.')
            continue;
        string folderpath = currworkdir+"/"+name;
        stat(&folderpath[0], &st);
        if((st.st_mode & S_IFMT)==S_IFDIR)
        {
            if(str==f->d_name)
                return true;
            directories.push_back(f->d_name);
        }
        else
        {
            if(str==f->d_name)
                return true;
        }
    }
    for(int i=0;i<directories.size();i++)
    {
        string xyz = currworkdir+"/"+directories[i];
        bool n = doSEARCH(str, xyz);
        if(n==true)
            return true;
    }
    return false;
}

#endif