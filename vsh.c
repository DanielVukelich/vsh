#include "vsh.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <assert.h>

#include "envmgr.h"
#include "argbuild.h"
#include "builtin.h"
#include "err_handling.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

int main(int __attribute__((unused))argc, char** argv){
    me = argv[0];
    running = 1;

    const long ARG_MAX = sysconf(_SC_ARG_MAX);
    char* cmdbuf = malloc(sizeof(char) * ARG_MAX);
    //This holds all the args that a child could be passed.
    //ARG_MAX is the maximum length of an argument, so there
    //can never be more than ARG_MAX/2 arguments (assuming there
    //are ARG_MAX arguments of 1 char each separated by spaces)
    char** child_argv = malloc(sizeof(char) * (ARG_MAX / 2));
    last_argv = child_argv;

    char user_prompt = '$';
    if(geteuid() == 0)
	user_prompt = '#';

    init_builtin_commands();
    init_envmgr();

    puts("Welcome to vsh, the Vukelich Shell.\n");

    while(running){

	printf(CYN "%s %c " RESET, get_pwd(), user_prompt);

	if(fgets(cmdbuf, ARG_MAX, stdin) != cmdbuf){
	    puts("");
	    running = 0;
	    continue;
	}

	arg_t* args = build_args(cmdbuf, ARG_MAX);

	//Get our argc and fill our argv
	int child_argc = get_args(args, child_argv);

	//See if the command we've been given should be
	//executed internally.  If not, assume it's a
	//program
	run_builtin_command(child_argc, child_argv);
	if(!builtin_cmd_found){
	    if(!fork()){
		execute_program(child_argv[0], child_argv);
		printf("%s: %s: Command not found\n", me, child_argv[0]);
		return 1;
	    }
	    wait(NULL);
	}

	//If the user ran a command built in to the shell
	//(like cd), and it had an error, print that error
	if(builtin_cmd_found && last_cmd_code){
	    print_error();
	}

	free_args(args);

    }
    puts("Exiting");
    free(cmdbuf);
    free(child_argv);
    deinit_builtin_commands();
    deinit_envmgr();

    return 0;
}
