#ifndef TASKCLI_H
#define TASKCLI_H

#include <stdio.h>
#include <string.h>

#include "task.h"

#define MAX_ARGS 30
#define MAX_ARG_LEN 60

int taskCli(int argc, char * argv[], task * taskList[], int * nextId, FILE * filePtr);
int cliCall(char stringToParse[], task * taskList[], int * nextId, FILE * filePtr);
int strToArg(int * argc, char * argv[], char * stringToParse);

#endif
