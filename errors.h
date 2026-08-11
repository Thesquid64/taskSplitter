typedef enum {
	E_INVALID_TASK_ID = 1, // First elem as 1
	E_TASK_NOT_FOUND = 2,	// Second as 2
	E_DEP_NOT_FOUND = 3,	// Etc ...
	E_DEP_ADD = 4,
	E_TASK_MALLOC_FAILED = 5,
	E_INVALID_TASK_PTR= 6,
	E_INVALID_TASKLIST_PTR = 7,
	E_INVALID_INT_PTR = 8,
	E_INVALID_FILE_PTR = 9,
	E_DELETED_TASK = 10,
	E_UNDEFINED = 11,
	E_INVALID_DEP = 12,
	E_CIRCULAR_DEP = 13,
	E_MISSING_QUOTE = 14,
	E_INVALID_COMMAND = 15,
	E_OK = 0, // 0 because normal functionnement
} common;
