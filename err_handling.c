#include "err_handling.h"
#include <stdio.h>
#include "vsh.h"

void print_error(){
  fprintf(stderr, "%s: %s: %s\n", me, last_argv[0], errstr);
}
