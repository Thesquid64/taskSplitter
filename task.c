#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "errors.h"
#include "flow.h"
#include "consts.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

int initFreeTask(task * taskList[], task ** taskPtrRef) {

	int i;

	if(taskList == NULL) return E_INVALID_TASKLIST_PTR;

	for(i=0; i<MAX_LOADED_TASKS; i++) {
		if(taskList[i] == NULL) {
			initTaskByRef(&taskList[i]);
			*taskPtrRef = taskList[i];
			return E_OK;
		}
	}

	return E_TASK_NOT_FOUND; // No free task found
}

int findFreeTask(task * taskList[], task ** taskPtrRef) {

	int i;

	if(taskList == NULL) return E_INVALID_TASKLIST_PTR;

	for(i=0; i<MAX_LOADED_TASKS; i++) {
		if(taskList[i] == NULL) {
			initTaskByRef(&taskList[i]);
			return E_OK;
		}
	}

	return E_TASK_NOT_FOUND; // No free task found
}

int initTaskByRef(task ** taskPtrRef) {

	int i;

	*taskPtrRef = malloc(sizeof(task));

	if(*taskPtrRef == NULL) return E_TASK_MALLOC_FAILED;

	strcpy((*taskPtrRef)->name, " ");
	strcpy((*taskPtrRef)->desc, " ");

	// Init all dependencies at none
	for(i=0; i<MAX_DEPENDENCIES; i++) {
		(*taskPtrRef)->dependencies[i] = -1;
	}

	(*taskPtrRef)->creation = time(NULL); // set the creation time

	(*taskPtrRef)->due = (*taskPtrRef)->creation + DEFAULT_DUE_INTERVAL; // set the default due time

	return E_OK;
}

int freeTask(task * taskPtr) {

	free(taskPtr);

	return E_OK;
}

int listTasks(task * taskList[], FILE * outFile) {

	int i;

	if(taskList == NULL) return E_INVALID_TASKLIST_PTR;
	if(outFile == NULL) return E_INVALID_FILE_PTR;

	for(i=0; i<MAX_LOADED_TASKS; i++) {
		printTask(taskList[i], outFile);
	}

	return E_OK;
}

int printTask(task * taskPtr, FILE * outFile) {

	int i;

	if(taskPtr == NULL) return E_INVALID_TASK_PTR;
	if(outFile == NULL) return E_INVALID_FILE_PTR;

	fprintf(outFile, "id:%d\n", taskPtr->id);

	if(strcmp(taskPtr->name, "") == 0) {
		fprintf(outFile, "name:\" \"\n");
	}
	else {
		fprintf(outFile, "name:\"%s\"\n", taskPtr->name);
	}

	if(strcmp(taskPtr->desc, "") == 0) {
		fprintf(outFile, "desc:\" \"\n");
	}
	else {
		fprintf(outFile, "desc:\"%s\"\n", taskPtr->desc);
	}

	fprintf(outFile, "dependencies:");
	fprintf(outFile, "[");

	i = 0;
	while(taskPtr->dependencies[i] == -1) {
		i++;
	}
	if(i < MAX_DEPENDENCIES) {
		fprintf(outFile,"%d", taskPtr->dependencies[i]);
		for(i=i+1; i<MAX_DEPENDENCIES; i++) {
			if(taskPtr->dependencies[i] != -1) fprintf(outFile,",%d", taskPtr->dependencies[i]);
		}
	}
	fprintf(outFile, "]\n");

	fprintf(outFile, "creation:%ld\n", taskPtr->creation);

	fprintf(outFile, "due:%ld\n", taskPtr->due);

	return E_OK;
}

int readTask(task * taskPtr, FILE * outFile) {

	int i;

	if(taskPtr == NULL) return E_INVALID_TASK_PTR;
	if(outFile == NULL) return E_INVALID_FILE_PTR;

	if(fscanf(outFile, "id:%d\n", &taskPtr->id) == -1) return E_TASK_NOT_FOUND;

	fscanf(outFile, "name:\"%[^\"]\"\n", taskPtr->name);

	fscanf(outFile, "desc:\"%[^\"]\"\n", taskPtr->desc);

	fscanf(outFile, "dependencies:[");
	for(i=0; i<MAX_DEPENDENCIES; i++) {
		fscanf(outFile,"%d", &taskPtr->dependencies[i]);
		fscanf(outFile,",");
	}
	fscanf(outFile, "]\n");

	fscanf(outFile, "creation:%ld\n", &taskPtr->creation);

	fscanf(outFile, "due:%ld\n", &taskPtr->due);

	return E_OK;
}

int saveTaskList(task * taskList[], int * nextId, FILE * filePtr) {

	if(taskList == NULL) return E_INVALID_TASKLIST_PTR;
	if(nextId == NULL) return E_INVALID_INT_PTR;
	if(filePtr == NULL) return E_INVALID_FILE_PTR;

	fprintf(filePtr, "nextId:%d\n", *nextId);

	listTasks(taskList, filePtr);

	return E_OK;
}

int loadTaskList(task * taskList[], int * nextId, FILE * filePtr) {

	int i;

	if(taskList == NULL) return E_INVALID_TASKLIST_PTR;
	if(nextId == NULL) return E_INVALID_INT_PTR;
	if(filePtr == NULL) return E_INVALID_FILE_PTR;

	fscanf(filePtr, "nextId:%d\n", nextId);

	for(i=0; i<MAX_LOADED_TASKS; i++) {

		initTaskByRef(&taskList[i]);
		if(readTask(taskList[i], filePtr) != E_OK) {
			taskList[i] = NULL;
		}
	}

	return E_OK;
}

int setNextTaskId(task * taskPtr, int * nextId) {

	if(taskPtr == NULL) return E_INVALID_TASK_PTR;
	if(nextId == NULL) return E_INVALID_INT_PTR;

	taskPtr->id = *nextId;
	(*nextId)++; // Increment id for future task creation

	return E_OK;
}

int findFreeDependency(task * taskPtr, int * outIndex) {

	int i;

	if(taskPtr == NULL) return E_INVALID_TASK_PTR;

	for(i=0; i<MAX_DEPENDENCIES; i++) {
		if(taskPtr->dependencies[i] == -1) {
			*outIndex = i;
			return E_OK;
		}
	}

	return E_DEP_NOT_FOUND;
}

int removeTaskDependency(task * taskPtr, task * taskDependency) {

	int i;

	if(taskPtr == NULL) return E_INVALID_TASK_PTR;

	for(i=0; i<MAX_DEPENDENCIES; i++) {
		if(taskPtr->dependencies[i] == taskDependency->id) {
			taskPtr->dependencies[i] = -1;
		}
	}

	return E_OK;
}

int addTaskDependency(task * taskPtr, task * taskDependency) { // Add a dependency to a task

	int i;
	int index;

	if(taskPtr == NULL) return E_INVALID_TASK_PTR;
	if(taskDependency == NULL) return E_INVALID_TASK_PTR;

	if(taskPtr->id == taskDependency->id) return E_CIRCULAR_DEP; // A task cannot be dependent on itself
	for(i=0; i<MAX_DEPENDENCIES; i++) {
		if(taskDependency->dependencies[i] == taskPtr->id) return -1; // Circular dependency case
	}

	if(findFreeDependency(taskPtr, &index) == E_DEP_NOT_FOUND) return E_DEP_ADD;

	taskPtr->dependencies[index] = taskDependency->id;

	return E_OK;
}

int removeTaskById(task * taskList[], int taskId) {

	int tempTaskIndex;
	int i = 0;

	task * tempTaskPtr;
	task * tempTaskPtr2;

	tempTaskPtr = NULL;
	tempTaskPtr2 = NULL;

	// Remove dependencies on this task in other tasks

	taskPtrById(taskList, taskId, &tempTaskPtr2);

	if(tempTaskPtr2 == NULL) {

		return E_TASK_NOT_FOUND;

	}

	for(i=0;i<MAX_LOADED_TASKS;i++) {
		if(taskList[i] != NULL) removeTaskDependency(taskList[i], tempTaskPtr2);
	}

	// Free the task

	taskIndexById(taskList, taskId, &tempTaskIndex);
	if(taskList[tempTaskIndex] == NULL) return E_INVALID_TASK_PTR;
	freeTask(taskList[tempTaskIndex]);
	taskList[tempTaskIndex] = NULL;

	return E_OK;
}

int taskIndexById(task * taskList[], int taskId, int * taskIndex) {

	int i;

	if(taskList == NULL) return E_INVALID_TASKLIST_PTR;

	for(i=0; i<MAX_LOADED_TASKS; i++) {
		if(taskList[i] != NULL && taskList[i]->id == taskId) {
			*taskIndex = i;
			return E_OK;
		}
	}

	return E_TASK_NOT_FOUND;
}

int taskPtrById(task * taskList[], int taskId, task ** taskPtrRef) {

	int i;

	if(taskList == NULL) return E_INVALID_TASKLIST_PTR;

	for(i=0; i<MAX_LOADED_TASKS; i++) {
		if(taskList[i] != NULL && taskList[i]->id == taskId) {
			*taskPtrRef = taskList[i];
			return E_OK;
		}
	}

	return E_TASK_NOT_FOUND;
}

int setDueTask(task * taskPtr, int day, int month, int year, int hour, int minutes) {

	if(taskPtr == NULL) return E_INVALID_TASK_PTR;

	struct tm time_info = {0};
	time_t epoch_time;

	time_info.tm_year = year - 1900; 
	time_info.tm_mon = month - 1;        
	time_info.tm_mday = day;
	time_info.tm_hour = hour;
	time_info.tm_min = minutes;
	time_info.tm_sec = 0;

	time_info.tm_isdst = -1;

	epoch_time = mktime(&time_info);

	taskPtr->due = epoch_time;

	return E_OK;

}

int coolPrint(task * taskPtr) {

	int i;
	struct tm timeInfo;
	time_t timeNow;

	if(taskPtr == NULL) return E_INVALID_TASK_PTR;

	(COLORS) ? printf("\033[7m%d\033[0m ", taskPtr->id) : printf("%d ", taskPtr->id);

		printf("[");

		i = 0;
		while(taskPtr->dependencies[i] == -1) {
			i++;
		}
		if(i < MAX_DEPENDENCIES) {
			printf("%d", taskPtr->dependencies[i]);
			for(i=i+1; i<MAX_DEPENDENCIES; i++) {
				if(taskPtr->dependencies[i] != -1) printf(",%d", taskPtr->dependencies[i]);
			}
		}

		printf("] | ");

	if(strcmp(taskPtr->name, " ") != 0) {
		(COLORS) ? printf("\033[1;4m%s\033[0m ", taskPtr->name) : printf("%s ", taskPtr->name);
	}

	time(&timeNow);

	if(taskPtr->due < timeNow) {
		(COLORS) ? printf("\033[41mPAST DUE\033[0m") : printf("PAST DUE");
	}

	if(strcmp(taskPtr->desc, " ") != 0) {
		(COLORS) ? printf("\n\033[2m%s\033[0m\n", taskPtr->desc) : printf("\n%s\n", taskPtr->desc);
	}

	timeInfo = *localtime(&taskPtr->creation);

	if(strcmp(taskPtr->desc, " ") == 0) printf("\n");

	printf("made %02d-%02d-%04d %02d:%02d ", 
			timeInfo.tm_mday, 
			timeInfo.tm_mon + 1, 
			timeInfo.tm_year + 1900, 
			timeInfo.tm_hour, 
			timeInfo.tm_min);

	timeInfo = *localtime(&taskPtr->due);

	printf("due %02d-%02d-%04d %02d:%02d", 
			timeInfo.tm_mday, 
			timeInfo.tm_mon + 1,     // Month is 0-11
			timeInfo.tm_year + 1900, // Year is years since 1900
			timeInfo.tm_hour, 
			timeInfo.tm_min);

	printf("\n"); 


	return E_OK;
}

int reIdTaskList(task * taskList[], int * nextId) {

	int temp = 0;
	int temp2 = 0;

	intDict *map = NULL;

	if(taskList == NULL) return E_INVALID_TASKLIST_PTR;

	flowSort(taskList);

	for(temp=0;temp<MAX_LOADED_TASKS;temp++) {
		if(taskList[temp] != NULL) {
			hmput(map, taskList[temp]->id, temp2); // Add key : temp -> value temp2 to dict "map"
			taskList[temp]->id = temp2; // Change id of task
			temp2++;
		}
	}

	*nextId = temp2; // Update nextId

	// Update ids of dependencies

	for(temp=0;temp<MAX_LOADED_TASKS;temp++) {
		if(taskList[temp] != NULL) {
			for(temp2=0;temp2<MAX_DEPENDENCIES;temp2++) {
				if(taskList[temp]->dependencies[temp2] != -1) {
					taskList[temp]->dependencies[temp2] = hmget(map, taskList[temp]->dependencies[temp2]); // Get value of key temp2
				}
			}
		}
	}

	return E_OK;

}
