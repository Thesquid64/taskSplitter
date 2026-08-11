#ifndef CLIMENU_H
#define CLIMENU_H

#define MAX_COMMAND_LEN 1000 
#define MAX_PATH_LEN 300

#include "task.h"

int cliMenu(task * taskList[], int * nextId);

#endif
