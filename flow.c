#include "task.h"
#include "consts.h"
#include "errors.h"

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

	// Put tasks A dependent on B before B
	
	for(i=0; i<MAX_LOADED_TASKS; i++) { // for each task
		if(taskList[i] == NULL) continue;
		for(j=i+1; j<MAX_LOADED_TASKS; j++) { // we search the first dependency of the task before the task
			if(taskList[j] == NULL) continue;
			for(k=0; k<MAX_DEPENDENCIES; k++) {
				if(taskList[j]->dependencies[k] == taskList[i]->id) { // If a dependency (first in taskList) is at j
					for(l=j; l>i; l--) { // we bubble down the dependency under the task
						tempTaskPtr = taskList[l];
                    				taskList[l] = taskList[l-1];
                    				taskList[l-1] = tempTaskPtr; 
					}

					i--; // As we moved i we need to recheck the dependencies at i
					goto rescan; // We already moved the dependency so we move on
				}
			}
		}
rescan: ;
	}

	return E_OK;
}	


int flowPrint(task * taskList[]) {

	int i;
	
	(COLORS) ? printf("\n\033[7mFLOW :                                         \033[0m\n\n") : printf("\nFLOW :                                         \n\n");

	for(i=0; i<MAX_LOADED_TASKS; i++) {
		if(taskList[i] != NULL) {
			coolPrint(taskList[i]);	

			printf("_______________________________________________");
			printf("\n\n");
		}
	}
	
	return E_OK;
}
