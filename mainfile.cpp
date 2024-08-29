#include "PWD.h"
#include "CD.h"
#include <regex>


using namespace std;

char buffer[100];
string getLoc(string currworkdir, string workingdir)
{
    if(currworkdir==workingdir)
            return "~";
        else if(currworkdir.length()>workingdir.length() && currworkdir.substr(0,workingdir.length())==workingdir)
            return "~"+currworkdir.substr(workingdir.length());
        else
            return currworkdir;
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
    string str;
    int val;
    do
    {
        currworkingdir = display(workingdir);
        getline(cin, command);
        if(command=="pwd")
        {
            getPWD();
            continue;
        }
        regex pattern("cd .*");
        if(regex_match(command, pattern))
        {
            CD(command.substr(3), currworkingdir, workingdir, prevdir);
            continue;
        }
        if(command=="exit")
            break;
    }
    while(true);
    return 0;
}