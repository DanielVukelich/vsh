#include "envmgr.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/limits.h>

static char* pwd;
static char* old_pwd;

static char prog_path[PATH_MAX];

//The compiler throws a hissyfit if setenv doesn't have a function declaration
//here, despite the fact that it *IS* in stdlib.h which *IS* #include'd
int setenv(const char*, const char*, int);

void init_envmgr(){
    pwd = malloc(sizeof(char) * PATH_MAX);
    if(getcwd(pwd, PATH_MAX) != pwd){
	pwd[0] = '\0';
    }
    old_pwd = malloc(sizeof(char) * PATH_MAX);
}

void deinit_envmgr(){
    free(pwd);
    free(old_pwd);
}

const char* get_pwd(){
    return pwd;
}

void update_pwd(){
    strncpy(old_pwd, pwd, PATH_MAX);
    if(getcwd(pwd, PATH_MAX) != pwd){
	pwd[0] = '\0';
    }
    setenv("PWD", pwd, 1);
    setenv("OLDPWD", old_pwd, 1);
}

void execute_program(char* program, char** arguments){
    //Try to execute the program in all the paths found in $PATH
    //But first, try to execute it in the current directory
    execv(program, arguments);
    //If we return from an execv call, it failed
    char* pathstr = getenv("PATH");

    if(!pathstr)
	return;

    char* begincolon = pathstr;
    char* endcolon = NULL;
    do{
	if(*pathstr == ':' || *pathstr == '\0'){
	    endcolon = pathstr;
	    int size = endcolon - begincolon;
	    strncpy(prog_path, begincolon, size);
	    //Add a slash to the end of the path before concatenating
	    //our command string
	    prog_path[size] = '/';

	    //Now add our command to the end
	    char* cat_begin = (prog_path + size + 1);
	    //don't let anyone copy past the end of the array
	    size = PATH_MAX - size - 1;
	    strncpy(cat_begin, program, size);

	    //Now try to run the program from that location
	    execv(prog_path, arguments);
	    begincolon = endcolon + 1;

	}
    }while(*(pathstr++) != '\0');

    //If we get to here, then the command could not be found
}
