#include <sys/ioctl.h>
#include <stdio.h>

#include "task.h"
#include "consts.h"
#include "errors.h"
#include "flow.h"

int flowSort(task * taskList[]) {

	int i, j, k, l;
	int taskLen;
	task * tempTaskPtr;

	// Sort by due date asc (bubble sort yayyyy)
	
	for(i=0; i<MAX_LOADED_TASKS-1; i++) {
		for(j=0; j<MAX_LOADED_TASKS -i -1; j++) {
			if (taskList[j] != NULL && taskList[j+1] != NULL) {
            			if (taskList[j]->due < taskList[j+1]->due) {
					tempTaskPtr = taskList[j];
                    			taskList[j] = taskList[j+1];
                    			taskList[j+1] = tempTaskPtr; 
				}
			}
		}
	}

	// Put dependency B dependent on A before A
	
	for(i=0; i<MAX_LOADED_TASKS; i++) { // for each task
		if(taskList[i] == NULL) continue;
		for(j=i+1; j<MAX_LOADED_TASKS; j++) { // we search the first dependency of the task sorted after the task
			if(taskList[j] == NULL) continue;
			for(k=0; k<MAX_DEPENDENCIES; k++) {
				if(taskList[j]->dependencies[k] == taskList[i]->id) { // If dependency B of A
					for(l=i; l<j; l++) { // we bubble down the dependency under the task
						tempTaskPtr = taskList[l];
                    				taskList[l] = taskList[l+1];
                    				taskList[l+1] = tempTaskPtr; 
					}

					j++; // As we moved j down in list we need to recheck the dependencies at j++
					i--; // i got down 1 in list because its depenency moved under i
					goto rescan; // We already moved the dependency so we move on
				}
			}
		}
rescan: ;
	}

	return E_OK;
}	


int flowPrint(task * taskList[]) {

	int i, j;
	int cols;

	struct winsize w;
    	ioctl(0, TIOCGWINSZ, &w);
    	w.ws_col != 0 ? cols = w.ws_col : (cols = 47);

	printf("\n");
	if(COLORS) printf("\033[7m");
	printf("FLOW :");
	for(j=0; j<cols-6; j++) {
		printf(" ");
	}
	if(COLORS) printf("\033[0m");
	printf("\n");
	printf("\n");

	for(i=0; i<MAX_LOADED_TASKS; i++) {
		if(taskList[i] != NULL) {
			coolPrint(taskList[i]);	

			for(j=0; j<cols; j++) {
				printf("_");
			}

			printf("\n\n");
		}
	}
	
	return E_OK;
}
