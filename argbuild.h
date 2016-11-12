#ifndef ARG_BUILD_H
#define ARG_BUILD_H

typedef struct arg_elt{
  struct arg_elt* next;
  char* arg;
  int arg_len;
} arg_elt_t;

typedef struct args{
  arg_elt_t* first;
  arg_elt_t* end;
  int argc;
} arg_t;

arg_t* build_args(char* cmd, unsigned int cmd_max_len);
int get_args(arg_t* args, char** argv_to_fill);
void free_args(arg_t* tofree);

#endif
