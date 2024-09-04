#ifndef LS_H
#define LS_H

#include<unistd.h>
#include<iostream>
#include<dirent.h>
#include<sys/stat.h>
#include <pwd.h>
#include <grp.h>

#include<regex>

using namespace std;

struct stat st;
struct group *grp;
struct passwd *pwd;

string printPermissions(struct stat st)
{
    string ret="";
    ret+=((st.st_mode & S_IFMT)==S_IFDIR ? "d" : "-");

    ret+=((st.st_mode & S_IRUSR) ? "r" : "-");
    ret+=((st.st_mode & S_IWUSR) ? "w" : "-");
    ret+=((st.st_mode & S_IXUSR) ? "x" : "-");
    ret+=((st.st_mode & S_IRGRP) ? "r" : "-");
    ret+=((st.st_mode & S_IWGRP) ? "w" : "-");
    ret+=((st.st_mode & S_IXGRP) ? "x" : "-");
    ret+=((st.st_mode & S_IROTH) ? "r" : "-");
    ret+=((st.st_mode & S_IWOTH) ? "w" : "-");
    ret+=((st.st_mode & S_IXOTH) ? "x" : "-");
    return ret;
}

/*
Modes:
1: ls
2: ls -l
3: ls -a
4: ls -la
*/

string LS_file(string &filepath, int mode)
{
    string ret="", t;
    int check = stat(&filepath[0], &st);
    if(check!=0)
    {
        perror("Invalid Input");
        return "";
    }
    if(mode==1 || mode==3)
    {
        return filepath;
    }
    else
    {
        pwd = getpwuid(st.st_uid);
        grp = getgrgid(st.st_gid);
        ret+=printPermissions(st);
        ret+=" ";
        ret+=to_string(st.st_nlink);
        ret+=" ";
        ret+=pwd->pw_name;
        ret+=" ";
        ret+=grp->gr_name;
        ret+=" ";
        ret+=to_string(st.st_size);
        ret+="\t";
        t = ctime(&st.st_mtime);
        ret+= t.substr(0, t.length()-1);
        ret+=" ";
        ret+=filepath;
    }
    return ret;
}

string LS_dir(string &dirpath, int mode)
{
    struct dirent *f;
    DIR *directory = opendir(&dirpath[0]);
    if(directory==NULL)
    {
        perror("Invalid Input");
        return "";
    }
    int total=0;
    regex pattern1("[a-zA-Z_0-9]+\\.[a-zA-Z_0-9]+");
    //todo:find the regex
    regex pattern2("\\.(.*)");
    //[a-zA-Z_\\ 0-9]+
    string filename, ret, temp, t;
    int check;
    while((f = readdir(directory))!= NULL)
    {
        filename="";
        temp = "";
        if(mode==1 || mode==2)
        {
            if(regex_match(f->d_name, pattern1) || !regex_match(f->d_name, pattern2))
                filename = f->d_name;
            else
                continue;
        }
        else
        {
            filename = f->d_name;
        }
        if(filename!="" && mode!=1 && mode!=3)
        {
            t = dirpath + "/" + filename;
            stat(&t[0], &st);
            total+=st.st_blocks;

            pwd = getpwuid(st.st_uid);
            grp = getgrgid(st.st_gid);
            temp+=printPermissions(st);
            temp+=" ";
            temp+=to_string(st.st_nlink);
            temp+=" ";
            temp+=pwd->pw_name;
            temp+=" ";
            temp+=grp->gr_name;
            temp+=" ";
            temp+=to_string(st.st_size);
            temp+=" ";
            t = ctime(&st.st_mtime);
            temp+= t.substr(0, t.length()-1);
            temp+=" ";
            ret+=temp;
        }
        ret += filename;
        ret+= "\n";
    }
    if(mode==2 || mode==4)
        ret = "total " + to_string(total/2) + "\n" + ret;
    return ret;
}

#endif