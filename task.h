#ifndef TASK_H
#define TASK_H

#define MAX_LOADED_TASKS 1000
#define TASK_NAME_LEN 50
#define TASK_DESC_LEN 200
#define MAX_DEPENDENCIES 100
#define DEFAULT_DUE_INTERVAL 604800 // 1 week

#include <time.h>

#include "stdio.h"

typedef struct task {
	int id;
	char name[TASK_NAME_LEN];
	char desc[TASK_DESC_LEN];
	int dependencies[MAX_DEPENDENCIES];
	time_t creation;
	time_t due;
} task;

typedef struct {
	int key;
	int value;
} intDict;

int initFreeTask(task * taskList[], task ** taskPtrRef); // Returns the index of the first free task in taskList
int findFreeTask(task * taskList[], task ** taskPtrRef); // Returns the index of the first free task in taskList
int initTaskByRef(task ** taskPtrRef); // Inits a task
int freeTask(task * taskPtr); // Free a task
int listTasks(task * taskList[], FILE * outFile); // Lists tasks in a taskList
int printTask(task * taskPtr, FILE * outFile); // prints info about a task from its pointer (empty str are replaced by a space)
int readTask(task * taskPtr, FILE * outFile); // reads a task to outFile from its pointer
int saveTaskList(task * taskList[], int * nextId, FILE * filePtr); // Saves a taskList to a file
int loadTaskList(task * taskList[], int * nextId, FILE * filePtr); // Loads a taskList from a file
int setNextTaskId(task * taskPtr, int * nextId); // Associate the next task id to taskPtr->id
int addTaskDependency(task * taskPtr, task * taskDependencyId); // Add a dependency to a task
int removeTaskDependency(task * taskPtr, task * taskDependency); // Remove a dependency from a task
int taskPtrById(task * taskList[], int taskId, task ** taskPtrRef); // Put the task with id "taskId" in taskList in taskPtr
int setDueTask(task * taskPtr, int day, int month, int year, int hours, int minutes);
int coolPrint(task * taskPtr);
int findFreeDependency(task * taskPtr, int * outIndex);
int taskIndexById(task * taskList[], int taskId, int * taskIndex); // find the task index in a taskList by a task id
int removeTaskById(task * taskList[], int taskId); // removes a task in a taskList by its id
int reIdTaskList(task * taskList[], int * nextId); // regenerates ids to be continuous from 0 (ex: 10, 14, 19 becomes 0, 1, 2)

#endif
