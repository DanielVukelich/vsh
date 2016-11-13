#ifndef ENVIRONMENT_MANAGER_H
#define ENVIRONMENT_MANAGER_H

#include <stdlib.h>

void init_envmgr();
void deinit_envmgr();

void update_pwd();
const char* get_pwd();
void execute_program(char* program, char** arguments);


#endif
