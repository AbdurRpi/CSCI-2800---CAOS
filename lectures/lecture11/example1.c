/*
Bash$ gcc -Wall -Werror lecture2.c
Bash$ gcc -Wall -Werror lecture2.c -lm
Bash$ gcc -E -Wall -Werror lecture2.c <== preprocessor only
*/

/* Inter- Process Communication (IPC)*/

/* To communicate with parent and child create 2 pipes and 2 forks
1 is for parent to child communication and the other foro child to parent communication
because a waitpid would require for a child process to terminate first.*/


// Quiz Questions
/* What is the file position going to inddex to?
Open a and Pipe go through the FD table and look for the next avaiable position*/
