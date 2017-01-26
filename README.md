# vsh
VSH:  The Vukelich Shell
VSH is a simple shell written in C.  It uses fork() and execute() to launch programs.  You can enter it and use it to run programs in a terminal, but it's quite limited otherwise.

What VSH does:

+ Execute programs in the current directory

+ Execute programs in the PATH
 
+ Change directories
    
+ Exit the shell

What VSH doesn't do:

+ Piping

+ Stdout redirection

+ Backgrounding/Foregrounding jobs

+ Shell scripting

+ Creating/modifying environment variables

+ Anything else not specified in the "What it does" section
