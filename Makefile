all: main.c task.o flow.o taskCli.o cliMenu.o
	gcc main.c task.o flow.o taskCli.o cliMenu.o -o taskSplitter -g -lreadline

install: all
	cp taskSplitter /bin

uninstall:
	rm /bin/taskSplitter

task.o: task.c task.h consts.h
	gcc -g -c task.c -o task.o

flow.o: flow.c flow.h consts.h
	gcc -g -c flow.c -o flow.o

taskCli.o : taskCli.c taskCli.h consts.h
	gcc -g -c taskCli.c -o taskCli.o

cliMenu.o : cliMenu.c cliMenu.h consts.h
	gcc -g -c cliMenu.c -o cliMenu.o 

clean:
	rm *.o > /dev/null 2>&1
	rm taskSplitter > /dev/null 2>&1
	rm saves/* > /dev/null 2>&1
