#include "PWD.h"
#include "CD.h"
#include "LS.h"
#include<regex>
#include<cstring>
#include<string>

using namespace std;

char buffer[100];
string getLoc(string currworkdir, string workingdir)
{
    if(currworkdir==workingdir)
            return "~";
        else if(currworkdir.length()>workingdir.length() && currworkdir.substr(0,workingdir.length())==workingdir)
            return "~"+currworkdir.substr(workingdir.length());
        else
            return "~"+currworkdir;
}

string display(string workingdir)
{
    getlogin_r(buffer, 100);
    cout << buffer;
    gethostname(buffer, 100);
    cout << "@" << buffer << ":";
    getcwd(buffer, 100);
    string currworkdir = buffer;
    cout << getLoc(currworkdir, workingdir) << ">";
    return currworkdir;
}

int main()
{
    getcwd(buffer, 100);
    string command, currworkingdir, workingdir = buffer, prevdir="";
    char *token;
    string str;
    int val;
    do
    {
        currworkingdir = display(workingdir);
        getline(cin, command);
        token = strtok(&command[0], " ");
        if(token)
        {
            if(!strcmp(token, "pwd"))
            {
                getPWD();
            }
            else if(!strcmp(token, "ls"))
            {
                vector<string> directories, files;
                int mode=1;
                string str, ret="";
                regex pattern1("-.*");
                bool flag = false;
                token = strtok(NULL," ");
                //collect the tokens into files and directories and get the mode
                while(token)
                {
                    str = token;
                    //checking for options
                    if(regex_match(str, pattern1))
                    {
                        if(str=="-l")
                        {
                            if(mode==3 || mode==4)
                                mode=4;
                            if(mode==1)
                                mode=2;
                        }
                        else if(str=="-a")
                        {
                            if(mode==2 || mode==4)
                                mode=4;
                            if(mode==1)
                                mode=3;
                        }
                        else if(str=="-al" || str=="-la")
                        {
                            mode=4;
                        }
                        else
                        {
                            str = "Invalid option for ls: " + str;
                            write(1,&str[0],str.length());
                            flag = true;
                            break;
                        }
                    }
                    //checking for ..
                    else if(str=="..")
                    {
                        
                        int loc = currworkingdir.substr(0,currworkingdir.length()-1).find_last_of("\\/");
                        if(loc!=-1)
                            str = currworkingdir.substr(0,loc);
                        else
                            str = currworkingdir;
                        directories.push_back(str);
                    }
                    //checking for ~
                    else if(str=="~")
                    {
                        directories.push_back(workingdir);
                    }
                    //checking for paths
                    else
                    {
                        struct stat st;
                        stat(&str[0], &st);
                        //check if it is a directory
                        if((st.st_mode & S_IFMT)==S_IFDIR)
                            directories.push_back(str);
                        else
                            files.push_back(str);
                    }
                    token = strtok(NULL," ");
                }
                if(directories.size()==0 && files.size()==0)
                    ret = LS_dir(currworkingdir, mode);
                else
                {
                    for(int i=0;i<files.size();i++)
                    {
                        ret+=LS_file(files[i], mode);
                        ret+="\n";
                    }
                    for(int i=0;i<directories.size();i++)
                    {
                        DIR *directory = opendir(&directories[i][0]);
                        if(directory==NULL)
                        {
                            string temp = directories[i] + ": Invalid Input";
                            perror(&temp[0]);
                            cout << "\n";
                            continue;
                        }
                        ret+=directories[i]+ ":\n";
                        ret+=LS_dir(directories[i], mode);
                        ret+="\n";
                    }
                    ret = ret.substr(0, ret.length()-1);
                }
                cout << ret;
            }
            else if(!strcmp(token,"cd"))
            {
                vector<string> tokens;
                token = strtok(NULL," ");
                while(token)
                {
                    tokens.push_back(token);
                    token = strtok(NULL," ");
                }
                if(tokens.size()>1)
                {
                    str = "Invalid Input: Too many arguments\n";
                    write(1,&str[0],str.length());
                }
                else if(tokens.size()==0)
                {
                    CD("~", currworkingdir, workingdir, prevdir);
                }
                else
                {
                    CD(tokens[0], currworkingdir, workingdir, prevdir);
                }
            }
            else if(!strcmp(token, "exit"))
                break;
            else
            {
                //todo: write execvp call
            }
        }
        
    }
    while(true);
    return 0;
}