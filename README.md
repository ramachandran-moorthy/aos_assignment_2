# POSIX Shell Implementation

**Assignment 2 - Advanced Operating Systems [Monsoon 2024]**

## Overview

This project implements a POSIX-compliant shell that supports various functionalities such as background processes, command execution, piping, input/output redirection, and more. The shell is designed to handle both internal and external commands in a modular manner.

## Features

1. **Custom Shell Prompt**: The shell displays the username, hostname, and current working directory, similar to a standard UNIX shell.
    - Prompt: `<ram_moorthy@Ram-Dell-Inspiron:~>`

2. **Built-in Commands**:
    - `cd`: Change the current directory with support for options like `..`, `-`, `~`.
    - `pwd`: Print the current working directory.
    - `echo`: Print text to stdout, supports spaces and tabs.
    - `ls`: List directory contents with options `-a`, `-l`, and handling of specific directories.

3. **System Commands**:
    - Execute system commands (e.g., `vim`, `emacs`, `gedit`) can be runin the foreground or background (`&` operator).
    - For background processes, the shell displays the PID of the process.


4. **Process Information** (`pinfo`):
    - Retrieve and display process information like PID, memory usage, and executable path.
    - `pinfo`: Prints the process-related info of your shell program.
    - `pinfo <pid>`: Prints the process-related info of your the program with given process id.

5. **I/O Redirection**:
    - **Input Redirection**: Using `<` to take input from a file instead of stdin.
    - **Output Redirection**: Using `>` to overwrite and `>>` to append to files.
    - Example: `sort < file1.txt > sorted.txt`

6. **Piping**:
    - The shell supports multiple commands piped together using the `|` symbol. 
    - Example: `cat file.txt | grep "pattern" | wc -l`

7. **Signal Handling**:
    - `CTRL+C`: Terminates foreground processes.
    - `CTRL+Z`: Moves foreground processes to the background.
    - `CTRL+D`: Exits the shell without closing the actual terminal

8. **Search**: Recursively search for a file in the current directory.
    - Example: `search file.txt`

9. **History**:
    - Stores up to 20 commands across sessions, and allows for browsing through history using the up and down arrow key.

10. **Tab Autocomplete**:
    - Pressing the TAB key either completes the command or outputs a list of matching files/dirs(space separated) if there are more than one.

## Compilation and Execution

### Build Instructions
The project uses a `makefile` for compilation. Ensure all source files are present in the working directory and run:

```bash
make
