#include "PWD.h"
#include "CD.h"
#include "LS.h"
#include "HISTORY.h"
#include "SEARCH.h"
#include "PINFO.h"
#include "SIGNALS.h"

#include<regex>
#include<string>
#include<sys/wait.h>
#include<termios.h>


using namespace std;

char buffer[100];

void display(string workingdir)
{
    getlogin_r(buffer, 100);
    cout << buffer;
    gethostname(buffer, 100);
    cout << "@" << buffer << ":";
    string cwd = getDisplayPath(workingdir);
    cout << cwd << ">";
}

struct termios setRawMode() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    return oldt;
}

void disableRawMode(struct termios oldt)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int main()
{
    getcwd(buffer, 100);
    string command, currworkingdir, workingdir = buffer, prevdir="", input="";
    char *token;
    vector<string> commands;
    string str;
    char ch;
    int val, historyIndex = -1;
    struct termios oldt = setRawMode();
    do
    {
        cout << "\r";
        commands.clear();
        display(workingdir);
        cout << input;
        cout.flush();
        currworkingdir = getPWD();
        vector<string> suggestions;
        DIR *directory = opendir(&currworkingdir[0]);
        struct stat st;
        struct dirent *f;

        signal(SIGINT, handle_sigint);   // CTRL-C
        signal(SIGTSTP, handle_sigtstp); // CTRL-Z

        read(STDIN_FILENO, &ch, 1);
        //ch = getchar();
        if(ch=='\t')
        {
            string part;
            int x = input.find_last_of(" ");
            if(x==-1)
                part = input;
            else
                part = input.substr(x+1);
            while((f = readdir(directory))!= NULL)
            {
                string name = f->d_name;
                if(name.length()>=part.length())
                {
                    if(name.substr(0,part.length())==part)
                    {
                        suggestions.push_back(name);
                    }
                }
            }
            if(suggestions.size()>1)
            {
                cout << "\n" << flush;
                for(int i=0;i<suggestions.size();i++)
                    cout << suggestions[i] << "\t" << flush;
                cout << "\n" << flush;
            }
            else if(suggestions.size()==0)
            {
                continue;
            }
            else
            {
                if(x==-1)
                    input = suggestions[0];
                else
                {
                    input = input.substr(0,x);
                    input += " " + suggestions[0];
                }
            }
        }
        else if(ch==127)//backspace
        {
            if (!input.empty())
            {
                input.pop_back();
                cout << "\b \b" << flush;
            }
        }
        else if(ch=='\x1b')//escape sequence
        {
            char seq[2];
            if(read(0, &seq[0], 1) == 0) break;
            if(read(0, &seq[1], 1) == 0) break;
            if (seq[0]=='[')
            {
                if (seq[1]=='A') // Up arrow key
                {
                    vector<string>history = getHISTORY(workingdir);
                    if(historyIndex==-1)
                        historyIndex = history.size();
                    
                    if(historyIndex>1)
                    {
                        historyIndex--;
                    }
                    else if(historyIndex==0)
                    {
                        cout << "\33[2K\r";
                        if(history.size()>0)
                            input = history[0];
                        //display(workingdir);
                        continue;
                    }
                    else
                    {
                        historyIndex = 0;
                    }
                    cout << "\33[2K\r";
                    //display(workingdir);
                    input = history[historyIndex];
                    cout.flush();

                }
                if (seq[1] == 'B') // Down arrow key
                {
                    vector<string>history = getHISTORY(workingdir);
                    if(historyIndex==-1)
                        historyIndex = history.size();
                    if(historyIndex<history.size()-1)
                    {
                        historyIndex++;
                    }
                    else if(historyIndex==history.size()-1)
                    {
                        cout << "\33[2K\r";
                        if(history.size()>0)
                            input = history[history.size()-1];
                        //display(workingdir);
                        continue;
                    }
                    else
                    {
                        historyIndex = history.size()-1;
                    }
                    cout << "\33[2K\r";
                    //display(workingdir);
                    input = history[historyIndex];
                    cout.flush();
                }
            }
        }
        else if(ch=='\n')
        {
            historyIndex=-1;
            cout << "\n";
            command = input;
            input="";
            if(command=="")
                continue;
            writeHISTORY(command, workingdir);
            int temp=0;
            for(int i=0;i<command.length();i++)
            {
                if(command[i]==';')
                {
                    commands.push_back(command.substr(temp,i-temp));
                    temp=i+1;
                }
            }
            commands.push_back(command.substr(temp, command.length()-temp));
            for(int i=0;i<commands.size();i++)
            {
                currworkingdir = getPWD();
                token = strtok(&commands[i][0], " ");
                if(token)
                {
                    if(!strcmp(token, "pwd"))
                    {
                        cout << getDisplayPath(workingdir) << "\n";
                        token = strtok(NULL," ");
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
                                    str = "Invalid option for ls: " + str + "\n";
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
                        if(flag==true)
                            continue;
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
                    else if(!strcmp(token, "echo"))
                    {
                        token = strtok(NULL, " \t");
                        while(token)
                        {
                            cout << token << " ";
                            token = strtok(NULL, " \t");
                        }
                        cout << "\n";
                    }
                    else if(!strcmp(token, "history"))
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
                            vector<string> commandQueue = getHISTORY(workingdir);
                            int n = min((int)commandQueue.size(), 10);
                            if(n>commandQueue.size())
                            {
                                n=commandQueue.size();
                            }
                            for(int i=commandQueue.size()-n;i<commandQueue.size();i++)
                            {
                                cout << commandQueue[i] << "\n";
                            }
                        }
                        else
                        {
                            regex numpattern("[0-9]*");
                            if(!regex_match(tokens[0], numpattern))
                            {
                                str = "Invalid Input: Incorrect argument\n";
                                write(1,&str[0],str.length());
                                continue;
                            }
                            int x = stoi(tokens[0]);
                            vector<string> commandQueue = getHISTORY(workingdir, x);
                            int n = min((int)commandQueue.size(), x);
                            if(n>commandQueue.size())
                            {
                                n=commandQueue.size();
                            }
                            for(int i=commandQueue.size()-n;i<commandQueue.size();i++)
                            {
                                cout << commandQueue[i] << "\n";
                            }
                        }
                    }
                    else if(!strcmp(token, "search"))
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
                            str = "Invalid Input: No file or directory provided\n";
                            write(1,&str[0],str.length());
                        }
                        else
                        {
                            bool ret = doSEARCH(tokens[0], currworkingdir);
                            if(ret==true)
                                cout << "True\n";
                            else
                                cout << "False\n";
                        }
                    }
                    else if(!strcmp(token, "pinfo"))
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
                            pid_t current_pid = getpid();
                            printProcessInfo(current_pid);
                        }
                        else
                        {
                            regex numpattern("[0-9]*");
                            if(!regex_match(tokens[0], numpattern))
                            {
                                str = "Invalid Input: Incorrect argument\n";
                                write(1,&str[0],str.length());
                                continue;
                            }
                            int x = stoi(tokens[0]);
                            printProcessInfo(x);
                        }
                    }
                    else if(!strcmp(token, "exit"))
                    {
                        disableRawMode(oldt);
                        exit(1);
                    }
                    else
                    {
                        vector<char*> tokens;
                        bool flag1 = false;
                        while(token)
                        {
                            string temp = token;
                            if(temp=="&"){
                                flag1 = true;
                                cout << "Background" << endl;
                            }
                            else
                                tokens.push_back(token);
                            token = strtok(NULL, " ");
                        }
                        tokens.push_back(nullptr);
                        int pid = fork();
                        if(pid==0)//child process
                        {
                            int val = execvp(tokens[0], tokens.data());
                            if(val==-1)
                                cout << "Invalid command: " << tokens[0] << "\n";
                            exit(1);
                        }
                        else //parent process
                        {
                            if(flag1==true)
                            {
                                cout << to_string(pid) << "\n";
                                flag1=false;
                            }
                            else
                            {
                                int status;
                                waitpid(pid, &status, WUNTRACED);
                            }
                        }
                    }
                }
            
            }
            }
        else if(ch==4)
        {
            disableRawMode(oldt);
            cout << "\n" << flush;
            exit(1);
        }
        else
        {
            input += ch;
            cout << ch << flush;
        }
        
    }
    while(true);
    disableRawMode(oldt);
    cout << flush;
    return 0;
}