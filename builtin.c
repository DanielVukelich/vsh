#include "builtin.h"

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "vsh.h"
#include "trie/trie.h"

static trie* cmd_map;

typedef int (*cmd_func_t)(int, char**);

typedef struct func_ary_elt{
  cmd_func_t function;
  char* cmd_name;
} func_ary_elt_t;

int shell_exit(int argc, char** argv){
  running = 0;
  return 0;
}

int change_directory(int argc, char** argv){
  if(argc == 1){
    //Return to ~ if home directory is set,
    //or do nothing
    puts("Home directory not set");
    return 0;
  }
  int toreturn = chdir(argv[1]);
  last_cmd_errno = errno;
  if(toreturn){
    errstr = strerror(last_cmd_errno);
  }
  return toreturn;
}

static func_ary_elt_t func_ary[] = {
  {&shell_exit, "exit"},
  {&change_directory, "cd"},
  {NULL, NULL}
};

void init_builtin_commands(){
  cmd_map = trie_create();
  for(func_ary_elt_t* i = func_ary; i->function != NULL; ++i){
    trie_insert(cmd_map, i->cmd_name, &(i->function));
  }
}

int run_builtin_command(int argc, char** argv){
  builtin_cmd_found = 1;
  //You can't cast a function pointer as void*, but you
  //can cast the pointer to a function pointer as void*
  cmd_func_t* to_run = (cmd_func_t*) trie_search(cmd_map, argv[0]);
  if(!to_run){
    builtin_cmd_found = 0;
    return 0;
  }
  cmd_func_t func_to_run = *to_run;
  last_cmd_code = func_to_run(argc, argv);
  return last_cmd_code;
}

void deinit_builtin_commands(){
  trie_free(cmd_map);
}
