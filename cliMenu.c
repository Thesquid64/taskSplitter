#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "taskCli.h"
#include "cliMenu.h"
#include "errors.h"
#include <readline/readline.h>
#include <readline/history.h>

int cliMenu(task * taskList[], int * nextId) {

	char * preCommand = "./taskSplitter -c ";
	char * firstCommand;
	char * command;
	char * input;
	int err;

	//                 0    5     11   16
	char commands[] = "open-close-quit-exit";
	char filePath[MAX_PATH_LEN];  

	int fileDescriptor;

	FILE * filePtr = NULL;

	int exitCheck = 0;

	while(!exitCheck) {
		input = readline("> ");
		if(strcmp(input, "") == 0) continue; // Handle empty inputs
		command = malloc(strlen(preCommand) + strlen(input) + 1);
		firstCommand = malloc(strlen(input) + 1);
		sscanf(input, "%s", firstCommand);

		if (strlen(input) > 0) {
			add_history(input);
		}

		switch(strstr(commands, firstCommand) - commands) {

			case 0 :

				sscanf(input, "open%s", filePath);
				filePtr = fopen(filePath, "r+");
				loadTaskList(taskList, nextId, filePtr);

				if(!filePtr) printf("File not found\n");
				else printf("File opened\n");

				break;

			case 5 :

				fflush(filePtr);
				fileDescriptor = fileno(filePtr);
				ftruncate(fileDescriptor, 0);
				rewind(filePtr);

				// Save changes
				saveTaskList(taskList, nextId, filePtr);
				fflush(filePtr);
				fclose(filePtr);
				filePtr = NULL;

				printf("File closed\n");

				break;

			case 11 :
			case 16 :

				exitCheck = 1;

				break;

			default : 

				if(!filePtr) {
					printf("Wrong command or no file opened yet !\n");
					break;
				}

				strcpy(command, preCommand);
				strcat(command, input);

				err = cliCall(command, taskList, nextId, filePtr);

				if(err == E_MISSING_QUOTE) {
					printf("Unexpected quote\n");
				}

				break;
		}

	}

	free(firstCommand);
	free(command);
	free(input);

	return 0;
}
