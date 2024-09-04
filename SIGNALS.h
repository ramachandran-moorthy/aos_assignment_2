#ifndef SIGNALS_H
#define SIGNALS_H

#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

pid_t fg_pid = -1; // Track the foreground process ID

void handle_sigint(int sig) {
    if (fg_pid > 0) {
        kill(fg_pid, SIGINT); // Send SIGINT to the foreground process
        fg_pid = -1;
    }
}

void handle_sigtstp(int sig) {
    if (fg_pid > 0) {
        kill(fg_pid, SIGTSTP); // Send SIGTSTP to the foreground process
        fg_pid = -1;
    }
}

#endif