#define MAXINPUTMENU 10
#define MAXCHOICEMENU 1

#include <time.h>
#include <stdlib.h>

#include "errors.h"
#include "task.h"
#include "taskCli.h"
#include "cliMenu.h"

struct tm splitTime = {0};

int main(int argc, char * argv[]) {

	printf("\n");
	printf("▀█▀ ▄▀█ █▀ █▄▀ █▀ █▀█ █░░ █ ▀█▀ ▀█▀ █▀▀ █▀█\n");
	printf("░█░ █▀█ ▄█ █░█ ▄█ █▀▀ █▄▄ █ ░█░ ░█░ ██▄ █▀▄\n\n");

	printf("Welcome to taskSplitter !");
	int i;
	int nextId;
	int exitCheck = 0; // false means do not exit
	task * loadedTasks[MAX_LOADED_TASKS]; // Store currently loaded tasks
	
	
	// Set to 0
	
	for(i=0; i<MAX_LOADED_TASKS; i++) {
		loadedTasks[i] = NULL;
	}

	nextId = 0;

	// Set up time related
	
	splitTime.tm_isdst = -1;

	// If there are arguments run as cli
	
	if(argc > 1) {
		taskCli(argc, argv, loadedTasks, &nextId, NULL);
		exit(EXIT_SUCCESS);
	}

	// Infinite menu
	int choice; // Store the user's choice number 
	char input[MAXINPUTMENU]; // Store the user's text input

	// Show choices to the user

	printf("\n\nChoose an menu :\n\n");
	printf("1 - Cli menu\n");
	// Set MAXCHOICE as the number of the last choice availible

	// Set choice to -1 in case user input is invalid
	choice = -1;

	// Get user choice while the input is wrong
	do {
		printf("\n> ");
		scanf("%d", &choice);
		while (getchar() != '\n');
	} while(choice < 0 || choice > MAXCHOICEMENU);

	printf("\n");

	switch(choice) {

		case 1 :

			printf("Menu's native commands : open [file], close, quit/exit\n");

			cliMenu(loadedTasks, &nextId);

			break;
	}
}
