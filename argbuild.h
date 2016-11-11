#ifndef ARG_BUILD_H
#define ARG_BUILD_H

typedef struct arg_elt{
  struct arg_elt* next;
  char* arg;
} arg_elt_t;

typedef struct args{
  arg_elt_t* first;
  arg_elt_t* end;
  int argc;
} arg_t;

arg_t* build_args(char* cmd, unsigned int cmd_max_len);
char** get_args(arg_t* args, int* numargs);
void free_args(arg_t* tofree);

#endif
