#ifndef HISTORY_H
#define HISTORY_H

#include<iostream>
#include<fstream>
#include<unistd.h>
#include<sys/stat.h>
#include<string>
#include<cstring>

using namespace std;

void getHISTORY(string workdir, int n=10)
{
    vector<string> commandQueue;
    ifstream histfile(workdir+"/history.txt");
    string buffer,  temp;
    while(getline(histfile, buffer))
    {
        commandQueue.push_back(buffer);
    }
    if(n>commandQueue.size())
    {
        n=commandQueue.size();
    }
    for(int i=commandQueue.size()-n;i<commandQueue.size();i++)
    {
        cout << commandQueue[i] << "\n";
    }
    histfile.close();
}

void writeHISTORY(string command, string workdir)
{
    vector<string> commandQueue;
    ifstream histfile(workdir + "/history.txt");
    string buffer, temp;
    int c=0;
    while(getline(histfile, buffer))
    {
        c++;
        commandQueue.push_back(buffer);
    }
    if(commandQueue.size()>=1)
    {
        if(commandQueue[commandQueue.size()-1]!=command)
            commandQueue.push_back(command);
    }
    else
        commandQueue.push_back(command);
    
    if(c>19)
    {
        commandQueue.erase(commandQueue.begin());
    }
    histfile.close();
    ofstream writetofile(workdir+"/history.txt");
    while(!commandQueue.empty())
    {
        writetofile << commandQueue.front() << "\n";
        commandQueue.erase(commandQueue.begin());
    }
    writetofile.close();
}

#endif