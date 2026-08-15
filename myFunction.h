#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define BUFF_SIZE 256

#define blue()    printf("\033[0;34m")
#define green()   printf("\033[0;32m")
#define bold()    printf("\033[1m")
#define reset()   printf("\033[0m")
#define boldOff() printf("\033[0m")

/* Input handling */
char *getInputFromUser(void);
char **splitArgument(char *str);
char *strwok(char *str, const char *delim);

/* Shell display */
void getLocation(void);

/* Shell commands */
void logout(char *input);
void echo(char **arg);
void echoppend(char **args);
void echorite(char **args);
void cd(char **arg);
void cp(char **arguments);
void delete(char **path);
void move(char **args);
void readd(char **args);
void wordCount(char **args);

/* Process execution */
void systemCall(char **arg);

/* Pipe handling */
char **splitInput(char *input, int *pipeIndex);
char **splitAfterPipe(char *input, int pipeIndex);
void mypipe(char **argv1, char **argv2);

#endif#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define BUFF_SIZE 256

#define blue()    printf("\033[0;34m")
#define green()   printf("\033[0;32m")
#define bold()    printf("\033[1m")
#define reset()   printf("\033[0m")
#define boldOff() printf("\033[0m")

/* Input handling */
char *getInputFromUser(void);
char **splitArgument(char *str);
char *strwok(char *str, const char *delim);

/* Shell display */
void getLocation(void);

/* Shell commands */
void logout(char *input);
void echo(char **arg);
void echoppend(char **args);
void echorite(char **args);
void cd(char **arg);
void cp(char **arguments);
void delete(char **path);
void move(char **args);
void readd(char **args);
void wordCount(char **args);

/* Process execution */
void systemCall(char **arg);

/* Pipe handling */
char **splitInput(char *input, int *pipeIndex);
char **splitAfterPipe(char *input, int pipeIndex);
void mypipe(char **argv1, char **argv2);

#endif
