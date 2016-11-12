#ifndef BUILTIN_H
#define BUILTIN_H

void init_builtin_commands();
int run_builtin_command(int argc, char** argv);
void deinit_builtin_commands();

#endif
