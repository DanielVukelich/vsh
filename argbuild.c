#include "argbuild.h"

#include <stdlib.h>
#include <string.h>

void add_arg(char* argument, arg_t* args, int len){
  if(args->end != NULL){
    arg_elt_t* last = args->end;
    int al = last->arg_len;
    if((al > 1) && (last->arg[al - 2] == '\\')){
      //If the last char of the last arg was backslash
      //then the slash should be replaced with
      //a space character and then the arg to add
      //should be added onto the end.
      last->arg[al - 2] = ' ';
      //Subtract one to account for removing 1 null
      //terminator after concatenating
      int newlen = al + len - 1;
      char* newarg = malloc(sizeof(char) * newlen);
      strncpy(newarg, last->arg, al);
      //Add the new argument onto the end
      char* newarg_offset = newarg + al - 1;
      strncpy(newarg_offset, argument, len);
      free(last->arg);
      last->arg = newarg;
      return;
    }
  }

  arg_elt_t* toadd = malloc(sizeof(arg_elt_t));
  toadd->arg = argument;
  toadd->next = NULL;
  toadd->arg_len = len;
  if(args->end != NULL){
    args->end->next = toadd;
  }
  args->end = toadd;
  if(args->first == NULL){
    args->first = toadd;
  }
  ++(args->argc);
}

arg_t* build_args(char* cmd, unsigned int cmd_max_len){
  arg_t* to_return = malloc(sizeof(arg_t));
  to_return->first = NULL;
  to_return->end = NULL;
  to_return->argc = 0;

  int sub_start = 0;
  int sub_end = 0;

  for(unsigned int i = 0; i < cmd_max_len; ++i){
    if(cmd[i] == '\0'){
      break;
    }
    if(cmd[i] == ' ' || cmd[i] == '\n'){

      //Ignore multiple spaces between arguments
      if(cmd[i] == ' ' && cmd[i + 1] == ' '){
	  continue;
      }

      char delineate = cmd[i];

      sub_end = i;
      int new_str_siz = (sub_end - sub_start) + 1;

      //Make sure the string is not empty
      if(new_str_siz > 1){
	//Temporarily add a null terminator in the cmd string to make
	//strncpy's job easier
	cmd[i] = '\0';
	char* arg_str = malloc(sizeof(char) * new_str_siz);
	//Copy the argument into the new string
	strncpy(arg_str, &cmd[sub_start], new_str_siz);
	//Add back in the space char that was replaced by null
	//terminator
	cmd[i] = delineate;
	//Now add the string to our linked list of arg elts
	add_arg(arg_str, to_return, new_str_siz);
	//Make sub_start point to the next char after cmd[i], which is
	//a space
	sub_start = i + 1;	
      }
    }
  }

  return to_return;
}

int get_args(arg_t* args, char** argv_to_fill){
  if(args->argc <= 0){
    return -1;
  }

  int numargs = 0;
  for(arg_elt_t* it = args->first; it != NULL; it = it->next){
    argv_to_fill[numargs++] = it->arg;

    //If the last non-null character in the last argument
    //is a backslash, replace it with a space
    if(it->next == NULL && it->arg[it->arg_len - 2] == '\\'){
      it->arg[it->arg_len - 2] = ' ';
    }

  }

  //The last elt in this array is a null pointer
  argv_to_fill[args->argc] = NULL;

  return args->argc;
}

void free_args(arg_t* tofree){
  arg_elt_t* it = tofree->first;
  while(it != NULL){
    arg_elt_t* next = it->next;
    if(it != NULL){
      free(it->arg);
    }
    free(it);
    it = next;
  }
  free(tofree);
}
