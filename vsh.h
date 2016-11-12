#ifndef VSH_H
#define VSH_H

//Argv[0] when this program was called.  The name this
//is being run as
char* me;
//The command that the user last tried to run
char** last_argv;
//True if we should continue to take user input
char running;
//True if the last builtin command we tried to
//run actually exists
char builtin_cmd_found;
//If a builtin command failed, set this to be the errno
//if what caused it to fail
int last_cmd_errno;
//A human readable string describing the errno from
//the previous command
char* errstr;

//The return code of the last command we tried to run
int last_cmd_code;

#endif
