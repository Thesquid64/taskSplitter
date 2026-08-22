#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "taskCli.h"
#include "task.h"
#include "flow.h"
#include "taskCli.h"
#include "errors.h"
#include "consts.h"

int taskCli(int argc, char * argv[], task * taskList[], int * nextId, FILE * filePtr) {

	//                 0       8      15     22    28    34        44   49   54   59  63       72   77    83     90   
	char commands[] = "addTask-rmTask-addDep-rmDep-print-coolPrint-flow-name-desc-due-flowSort-reId-clear-search-help";

	int fileSet = 1;
	int currArgIndex = 1;
	int temp, temp2, i;
	task * tempTaskPtr;
	task * tempTaskPtr2;
	task ** tempTaskPtrRef;
	int fileDescriptor;
	char tempStr[TASK_DESC_LEN];
	int err;
	int cols;
	struct winsize w;

	int datePlus, dateMinus;
	time_t tempTime;
	struct tm *currentTime;
	int day, month, year, hour, minutes;

	// Initialise values at defaults

	tempTime = time(NULL);
	currentTime = localtime(&tempTime);

	day = currentTime->tm_mday;
	month = currentTime->tm_mon + 1;
	year = currentTime->tm_year + 1900;
	hour = currentTime->tm_hour;
	minutes = currentTime->tm_min;

	if(!filePtr && strcmp(argv[currArgIndex], "-f") == 0) {
		fileSet = 0;
		currArgIndex++;
		filePtr = fopen(argv[currArgIndex], "r+");
		if(!filePtr) {
			printf("File not found\n");
			return 1;
		}

		loadTaskList(taskList, nextId, filePtr);
		currArgIndex++;
	}


	while(currArgIndex < argc && strcmp(argv[currArgIndex], "-c") == 0) {

		currArgIndex++;

		if(!strstr(commands, argv[currArgIndex])) {
			printf("Command not found\n");
			return E_INVALID_COMMAND;
		}

		switch(strstr(commands, argv[currArgIndex]) - commands) {

			case 0 :

				if(initFreeTask(taskList, &tempTaskPtr) != E_OK) {
					printf("Error while creating task\n");
					break;
				}
				setNextTaskId(tempTaskPtr, nextId);
				printf("Task created with id %d\n", *nextId -1);
				currArgIndex++;

				break;

			case 8 :

				if(currArgIndex +1 >= argc) {
					printf("Not enough arguments for rmTask\n");
					return 1;
				}
				if(sscanf(argv[currArgIndex+1], "%d", &temp) == 1) {
					if(removeTaskById(taskList, temp) != E_OK) {
						printf("Error while removing task\n");
						break;
					}
					printf("Task removed\n");
				}
				else {
					printf("Not a task id\n");
				}
				currArgIndex++;

				break;

			case 15 :

				if(currArgIndex +2 >= argc) {
					printf("Not enough arguments for addDp\n");
					return 1;
				}
				if(sscanf(argv[currArgIndex+1], "%d", &temp) == 1 && sscanf(argv[currArgIndex + 2], "%d", &temp2) == 1) {
					if(taskPtrById(taskList, temp, &tempTaskPtr) != E_OK) {
						printf("Invalid first task\n");
						break;
					}
					if(taskPtrById(taskList, temp2, &tempTaskPtr2) != E_OK) {
						printf("Invalid second task\n");
						break;
					}
					if(addTaskDependency(tempTaskPtr, tempTaskPtr2) != E_OK) {
						printf("Error while adding dependency\n");
						break;
					};
					printf("Dependency added\n");
				}
				else {
					printf("Not a task id\n");
				}
				currArgIndex += 2;

				break;

			case 22 :

				if(currArgIndex +2 >= argc) {
					printf("Not enough arguments for rmDp\n");
					return 1;
				}
				if(sscanf(argv[currArgIndex+1], "%d", &temp) == 1 && sscanf(argv[currArgIndex + 2], "%d", &temp2) == 1) {
					if(taskPtrById(taskList, temp, &tempTaskPtr) != E_OK) {
						printf("Invalid first task\n");
						break;
					}
					if(taskPtrById(taskList, temp2, &tempTaskPtr2) != E_OK) {
						printf("Invalid second task\n");
						break;
					}
					if(removeTaskDependency(tempTaskPtr, tempTaskPtr2) != E_OK) {
						printf("Error while removing dependency");
						break;
					}
					printf("Dependency removed\n");
				}
				else {
					printf("Not a task id\n");
				}
				currArgIndex += 2;

				break;

			case 28 :

				if(currArgIndex +1 >= argc) {
					printf("Not enough arguments for print\n");
					return 1;
				}
				if(sscanf(argv[currArgIndex+1], "%d", &temp) == 1) {
					if(taskPtrById(taskList, temp, &tempTaskPtr) != E_OK) {
						printf("Error while printing task\n");
						break;
					}
					printTask(tempTaskPtr, stdout);
				}
				else {
					printf("Not a task id\n");
				}
				currArgIndex += 1;

				break;

			case 34 :

				if(currArgIndex +1 >= argc) {
					printf("Not enough arguments for coolPrint\n");
					return 1;
				}
				if(sscanf(argv[currArgIndex+1], "%d", &temp) == 1) {
					if(taskPtrById(taskList, temp, &tempTaskPtr) != E_OK) {
						printf("Error while coolPrinting task\n");
						break;
					}
					coolPrint(tempTaskPtr);
				}
				else {
					printf("Not a task id\n");
				}
				currArgIndex += 1;

				break;

			case 44 :

				if(currArgIndex >= argc) {
					printf("Not enough arguments for flow\n");
					return 1;
				}

				if(flowSort(taskList) != E_OK) {
					printf("Error while sorting tasks\n");
					break;
				}
				if(flowPrint(taskList) != E_OK) {
					printf("Error while printing tasks\n");
					break;
				}

				currArgIndex += 1;

				break;

			case 49 :

				currArgIndex++;
				if(currArgIndex + 1 >= argc) {
					printf("Not enough arguments for name\n");
					break;
				}

				if(sscanf(argv[currArgIndex], "%d", &temp) != 1) {
					printf("Not a task id\n");
					break;
				}

				currArgIndex++;

				if(strlen(argv[currArgIndex]) == 0) {
					printf("Empty names are not allowed. Make it a space (\" \")\n");
					break;
				}
				if(strchr(argv[currArgIndex], '\n') != NULL) {
					printf("No newline allowed in names\n");
					break;
				}
				if(taskPtrById(taskList, temp, &tempTaskPtr) == E_TASK_NOT_FOUND) {
					printf("Task not found\n");
					break;
				}

				strcpy(tempTaskPtr->name, argv[currArgIndex]);

				printf("Name modified\n");

				currArgIndex++;

				break;

			case 54 :

				currArgIndex++;

				if(currArgIndex + 1 >= argc) {
					printf("Not enough arguments for description\n");
					break;
				}

				if(sscanf(argv[currArgIndex], "%d", &temp) != 1) {
					printf("Not a task id\n");
					return 1;
				}

				currArgIndex++;

				if(strlen(argv[currArgIndex]) == 0) {
					printf("Empty descriptions are not allowed. Make it a space (\" \")\n");
					break;
				}
				if(strchr(argv[currArgIndex], '\n') != NULL) {
					printf("No newline allowed in desc\n");
					return 1;
				}
				if(taskPtrById(taskList, temp, &tempTaskPtr) == E_TASK_NOT_FOUND) {
					printf("Task not found\n");
					break;
				}

				strcpy(tempTaskPtr->desc, argv[currArgIndex]);

				printf("Desc modified\n");

				currArgIndex++;

				break;

			case 59 :

				currArgIndex++;

				if(sscanf(argv[currArgIndex], "%d", &temp2) != 1) {
					printf("Not a task id\n");
					return 1;
				}

				currArgIndex++;

				while(currArgIndex < argc) {

					datePlus = 0;
					dateMinus = 0;

					if(argv[currArgIndex][0] == '+') {
						if(sscanf(argv[currArgIndex], "+%d", &temp) != 1) {
							printf("Error in date\n");
						}
						datePlus = 1;
					}				

					else if(sscanf(argv[currArgIndex], "%d", &temp) != 1) {
						printf("Error in date\n");
					}

					switch (argv[currArgIndex][strlen(argv[currArgIndex]) - 1]) {

						case 'D' : 

							if(datePlus) day += temp;
							else day = temp;

							break;

						case 'M' :

							if(datePlus) month += temp;
							else month = temp;

							break;

						case 'Y' :

							if(datePlus) year += temp;
							else year = temp;

							break;

						case 'h' :

							if(datePlus) hour += temp;
							else hour = temp;

							break;

						case 'm' :

							if(datePlus) minutes += temp;
							else minutes = temp;

							break;
					}

					currArgIndex++;
				}

				err = taskPtrById(taskList, temp2, &tempTaskPtr);
				if(err != E_OK) {
					printf("Not a task id\n");
					break;
				}
				setDueTask(tempTaskPtr, day, month, year, hour, minutes);

				printf("Date modified\n");

				break;

			case 63 : 

				if(flowSort(taskList) != E_OK) {
					printf("Error while sorting tasks");
				}

				break;

			case 72 :

				if(reIdTaskList(taskList, nextId) != E_OK) {
					printf("Error while reIding");
					break;
				}
				printf("reId done\n");

				break;

			case 77 :

				printf("\033[2J");

				currArgIndex++;

				break;

			case 83 :

				if(currArgIndex +1 >= argc) {
					printf("Not enough arguments for search\n");
					return 1;
				}

    				ioctl(0, TIOCGWINSZ, &w);
    				w.ws_col != 0 ? cols = w.ws_col : (cols = 47);

				printf("\n");
				if(COLORS) printf("\033[7m");
				printf("SEARCH RESULTS :");
				for(i=0; i<cols-16; i++) {
					printf(" ");
				}
				if(COLORS) printf("\033[0m");
				printf("\n");
				printf("\n");

				for(temp=0; temp<MAX_LOADED_TASKS; temp++) {
					if(taskList[temp] != NULL && strstr(taskList[temp]->name, argv[currArgIndex + 1]) != NULL) {
						coolPrint(taskList[temp]);	

						for(i=0; i<cols; i++) {
							printf("_");
						}
						printf("\n\n");
					}
				}

				currArgIndex += 2;

				break;

			case 90 :

				printf("Availible commands :\n\
\n\
	- addTask : Adds a task with auto incrementing id number\n\
	- rmTask taskId : Removes the task \"taskId\"\n\
	- addDep taskId depId : Adds the task \"depId\" as a dependency of task taskId\n\
	- rmDep taskId depId : Removes the task \"depId\" as a dependency of task taskId\n\
	- print taskId : Print info about task taskId as written in the savefile\n\
	- coolPrint taskId : Print info about a task taskId in a readable way\n\
	- flow : Print all tasks with coolPrint style ordered by due date asc (top to bottom) and with dependencies under their mother task \n\
	- name taskId taskName : Sets the name of task taskId as taskName\n\
	- desc taskId taskDesc : Sets the description of task taskId as tasskDesc\n\
	- due taskId [[+]Ya] [[+]Mb] [[+]Dc] [[+]hd] [[+]me] : Sets the due date of task taskId with year a, month b, day c, hour d, minutes e\n\
	- flowSort : Sorting of flow without printing\n\
	- reId : Rearranges the id of all tasks\n\
	- clear : Clears the screen\n\
	- search yourSearch : Prints tasks that contain yourSearch with coolPrint style\n\
	- help : Print this help\n");

				break;

			default :

				printf("\"%s\" is not a valid command\n", argv[currArgIndex]);
				// printf("%d\n", strstr(commands, argv[currArgIndex]) - commands);
				return 1;

				break;
		}
	}

	//listTasks(taskList, stdout); // for debug

	// Clear file for rewriting and go back to start

	if(!fileSet) {

		fflush(filePtr);
		fileDescriptor = fileno(filePtr);
		ftruncate(fileDescriptor, 0);
		rewind(filePtr);

		// Save changes
		saveTaskList(taskList, nextId, filePtr);
		fflush(filePtr);
		fclose(filePtr);
	}

	return 0;
}

int strToArg(int * argc, char * argv[], char strToParse[]) {

	int i;
	int strLen;
	int start = 0;
	int len;
	int currArg = 0;

	strLen = strlen(strToParse);
	*argc = 0;

	for(i=0; i<strLen; i++) {
		if(strToParse[i] == '\"') {
			i++;
			start = i;
			while(i<strLen && strToParse[i] != '\"') i++;
			if(i >= strLen) return E_MISSING_QUOTE; 
			len = i-start;
			strncpy(argv[currArg], &strToParse[start], len);
			argv[currArg][len] = '\0';
			i++;
		}
		else {
			start = i;
			while(i < strLen && strToParse[i] != ' ') i++;
			len = i - start;
			strncpy(argv[currArg], &strToParse[start], len);
			argv[currArg][len] = '\0';
		}
		currArg++;
	}
	*argc = currArg;
	argv[currArg] = NULL;
	return E_OK;
}

int cliCall(char stringToParse[], task * taskList[], int * nextId, FILE * filePtr) {

	char * argv[MAX_ARGS];
	int argc;
	int i;
	int err;

	for (int i = 0; i < MAX_ARGS; i++) {
		argv[i] = malloc(MAX_ARG_LEN);
	}

	err = strToArg(&argc, argv, stringToParse);
	if(err != E_OK) return err;

	taskCli(argc, argv, taskList, nextId, filePtr);

	for (int i = 0; i < MAX_ARGS; i++) {
		free(argv[i]);
	}

	return E_OK;
}
