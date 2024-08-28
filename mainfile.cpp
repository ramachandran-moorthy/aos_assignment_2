#include "PWD.h"

using namespace std;

char buffer[100];
string getLoc(string currworkdir, string workingdir)
{
    if(currworkdir==workingdir)
            return "~";
        else if(currworkdir.length()>workingdir.length() && currworkdir.substr(0,workingdir.length())==workingdir)
            return currworkdir.substr(workingdir.length());
        else
            return currworkdir;
}

void display(string workingdir)
{
    getlogin_r(buffer, 100);
    cout << buffer;
    gethostname(buffer, 100);
    cout << "@" << buffer << ":";
    getcwd(buffer, 100);
    string currworkdir = buffer;
    cout << getLoc(currworkdir, workingdir) << ">";
}

int main()
{
    getcwd(buffer, 100);
    string command, workingdir = buffer;
    do
    {
        display(workingdir);
        getline(cin, command);
        if(command=="pwd")
        {
            string str = getPWD();
            str+="\n";
            write(1, &str[0], str.length());
            continue;
        }

        if(command=="exit")
            break;
    }
    while(true);
    return 0;
}