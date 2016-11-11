#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <assert.h>

#include "argbuild.h"

#define DIR_STR_SIZ 1024

int main(int argc, char** argv, char** envp){
  char running = 1;
  char pwd[DIR_STR_SIZ];
  const long ARG_MAX = sysconf(_SC_ARG_MAX);
  char cmdbuf[ARG_MAX];

  char user_prompt = '$';
  if(geteuid() == 0)
    user_prompt = '#';

  puts("Welcome to vsh, the Vukelich Shell.\n");


  while(running){
    
    //Get the current working directory.  If an error occurs, then
    //set the pwd string to empty
    if(getcwd(pwd, DIR_STR_SIZ) != pwd){
      pwd[0] = '\0';
    }

    printf("%s %c ", pwd, user_prompt);

    if(fgets(cmdbuf, ARG_MAX, stdin) != cmdbuf){
      puts("\nExiting");
      running = 0;
      continue;
    }

    arg_t* args = build_args(cmdbuf, ARG_MAX);
    int child_argc;
    char** arg_ary = get_args(args, &child_argc);

    if(!fork()){
      execve(arg_ary[0], arg_ary, envp);
      printf("%s: %s: Command not found\n", argv[0], arg_ary[0]);
      return 1;
    }
    wait(NULL);
    free(arg_ary);
    free_args(args);

  }

  return 0;
}
