#ifndef LS_H
#define LS_H

#include<unistd.h>
#include<iostream>
#include<dirent.h>
#include<sys/stat.h>
#include<regex>

using namespace std;

struct dirent *f;
struct stat st;

void printPermissions(struct stat st)
{
    //todo: directory check
    //cout << ((st.st_mode & S_IRUSR) ? "d" : "-");

    cout << ((st.st_mode & S_IRUSR) ? "r" : "-");
    cout << ((st.st_mode & S_IWUSR) ? "w" : "-");
    cout << ((st.st_mode & S_IXUSR) ? "x" : "-");
    cout << ((st.st_mode & S_IRGRP) ? "r" : "-");
    cout << ((st.st_mode & S_IWGRP) ? "w" : "-");
    cout << ((st.st_mode & S_IXGRP) ? "x" : "-");
    cout << ((st.st_mode & S_IROTH) ? "r" : "-");
    cout << ((st.st_mode & S_IWOTH) ? "w" : "-");
    cout << ((st.st_mode & S_IXOTH) ? "x" : "-");

}

void LS(string filepath, int mode)
{
    DIR *directory = opendir(&filepath[0]);
    regex pattern1("[a-zA-Z_0-9]+\\.[a-zA-Z_0-9]+");
    regex pattern2("[a-zA-Z_\\ 0-9]+");
    string filename;
    while((f = readdir(directory))!= NULL)
    {
        filename="";
        if(mode==1 || mode==2)
        {
            if(regex_match(f->d_name, pattern1))
                filename = f->d_name;
            else if(regex_match(f->d_name, pattern2))
            {
                filename = "'";
                filename += f->d_name;
                filename += "'";
            }
            else
                continue;
        }
        else
        {
            filename = f->d_name;
        }
        if(filename!="")
        {
            stat(&filepath[0], &st);
            printPermissions(st);
        }

    }
    cout << "\n";
}

#endif