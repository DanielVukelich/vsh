#include "vsh.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <assert.h>

#include "argbuild.h"
#include "builtin.h"
#include "err_handling.h"

#define DIR_STR_SIZ 1024

int main(int argc, char** argv, char** envp){
  me = argv[0];
  running = 1;
  char pwd[DIR_STR_SIZ];
  const long ARG_MAX = sysconf(_SC_ARG_MAX);
  char cmdbuf[ARG_MAX];
  //This holds all the args that a child could be passed.
  //ARG_MAX is the maximum length of an argument, so there
  //can never be more than ARG_MAX/2 arguments (assuming there
  //are ARG_MAX arguments of 1 char each separated by spaces)
  char* child_argv[ARG_MAX / 2];
  last_argv = child_argv;

  char user_prompt = '$';
  if(geteuid() == 0)
    user_prompt = '#';

  init_builtin_commands();

  puts("Welcome to vsh, the Vukelich Shell.\n");


  while(running){
    
    //Get the current working directory.  If an error occurs, then
    //set the pwd string to empty
    if(getcwd(pwd, DIR_STR_SIZ) != pwd){
      pwd[0] = '\0';
    }

    printf("%s %c ", pwd, user_prompt);

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
	execve(child_argv[0], child_argv, envp);
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
  
  deinit_builtin_commands();

  return 0;
}
