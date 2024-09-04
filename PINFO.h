#ifndef PINFO_H
#define PINFO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

string getProcessStatus(char state)
{
    switch (state) {
        case 'R': return "Running";
        case 'S': return "Sleeping";
        case 'D': return "Waiting";
        case 'Z': return "Zombie";
        case 'T': return "Stopped";
        default: return "Unknown";
    }
}

string readFile(const string &filePath) {
    ifstream file(filePath);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

string getExecutablePath(pid_t pid) {
    char path[PATH_MAX];
    ssize_t len = readlink(("/proc/" + to_string(pid) + "/exe").c_str(), path, sizeof(path) - 1);
    if (len != -1) {
        path[len] = '\0';
        return string(path);
    } else {
        return "Unknown";
    }
}

void printProcessInfo(pid_t pid) {
    string statusFilePath = "/proc/" + to_string(pid) + "/stat";
    string statusContent = readFile(statusFilePath);
    
    if (statusContent.empty()) {
        cerr << "Could not retrieve information for PID " << pid << endl;
        return;
    }

    istringstream ss(statusContent);
    string token;
    vector<string> tokens;

    while (ss >> token) {
        tokens.push_back(token);
    }

    char state = tokens[2][0];
    long memory = stol(tokens[22]);
    string exePath = getExecutablePath(pid);
    string plus = pid==getpid()?"+":"";

    cout << "pid -- " << pid << "\n";
    cout << "Process Status -- " << state << plus << "\n";
    cout << "Memory -- " << memory << "\n";
    cout << "Executable Path -- " << exePath << "\n";
}

#endif