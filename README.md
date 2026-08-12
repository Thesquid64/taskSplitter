<pre>▀█▀ ▄▀█ █▀ █▄▀ █▀ █▀█ █░░ █ ▀█▀ ▀█▀ █▀▀ █▀█
░█░ █▀█ ▄█ █░█ ▄█ █▀▀ █▄▄ █ ░█░ ░█░ ██▄ █▀▄</pre>


**A mix of an agenda and a to do list without the cons**

## About

taskSplitter is a CLI and CLI like menu organisation tool that works like a merge of an agenda and a to do list.
You can create tasks that have amongst others, a due date and dependencies.
This permits an order of tasks in time as in an agenda but they don't have a start time as in a to do list.
This removes the need to wait for the next task as in a to do list.
Also, as you do not have to specify a date as in an agenda and can optionally use the dependency system, you can add tasks for everything without caring, that are automatically due 7 days later, and not forget about them.

## Getting started

Installation on Windows, Mac OS and other OSes surely is also easy but i havent had the time to make a guide for it yet !

Here are the instructions to install taskSplitter on Linux systems :

Precise instruction for installing dependencies will be shown for Ubuntu, Void Linux and Termux.

Using a different distro ?

Dependencies are surely also availible with your distro's package manager, maybe under another name.
A quick search online should do the trick (build the dependencies yourself in last resort).

### Prerequisites

***Notice :***

***At the start of a command example :***

***'$' means the command is executed with normal privileges***

***'#' means the command is executed with root privileges (search sudo if you dont understand this)***

#### Installing dependencies

taskSplitter has 2 dependencies :

- readline
- stb_ds.h (ALREADY INCLUDED IN SOURCE FILES)

To install it, you can use the package manager of your linux distro if it is availible.
Here are some examples :

Ubuntu (apt) (TO BE CHECKED) :

```# apt install libreadline8 libreadline-dev```

Void Linux (xbps) :

```# xbps-install readline readline-dev```

Termux android (apt) :

```$ apt install readline``` 

#### Installing build tools

To compile (build) taskSplitter you will need two programs :

- GCC or clang (depending on the distribution)
- make

To install GCC/clang and make :

Ubuntu (apt) (TO BE CHECKED) :

```# apt install gcc make```

Void Linux (xbps):

```# xbps-install gcc make```

Termux android (apt) :

```$ apt install clang make```

#### Installing git

Ubuntu (apt) (TO BE CHECKED) :

```# apt install git```

Void Linux (xbps):

```# xbps-install git```

Termux android (apt) :

```$ apt install git```

### Cloning the repo

In the directory you want taskSplitter's source files to be downloaded, run :

```$ git clone https://Thesquid64/taskSplitter.git```

### Building

Enter the cloned repo :

```$ cd taskSplitter```

If your terminal doesn't suppor colors (ANSI codes), you can disable them by setting COLORS to 0 in consts.h file

```#define COLORS 0```

Build using make :

```$ make```

Or install system-wide :

```# make install```

The program is built and ready to run ! if there are errors, check if you have installed or updated the required dependencies.

Then to run :

In the directory where the program was built :

```$ ./taskSplitter```

If installed system-wide :

```$ taskSplitter```

In this guide, all examples will be shown for local builds. They can be adapted by removing "./" at the start of commands for system-wide (for cli only).

## Usage

***Notice :***

***This usage guide will walk you trough how to use this tool, but also how i intend the tool to be used, as i made it to be very versatile, and maybe you will discover your own way to use it !***

You can use taskSplitter as a cli and with cliMenu.

cliMenu is just an easier way to use the cli, to not have to specify the file each time, so the commands are the same so here are explained the usages specific to the cli and the ones specific to cliMenu.

### cli

The cli is used directily from the terminal window this way :

```$ ./taskSplitter -f [filepath] -c [command]```

The addTask command is explained later but as an example, if i want to addTask a task to the file at path $HOME/myTaskFile i would use the command :

```$ ./taskSplitter -f $HOME/myTaskFile -c addTask```

Relative filepaths are also accepted, so if i already am in '$HOME', i can also do :

```$ ./taskSplitter -f myTaskFile -c addTask```

With the cli you can easily create bash scripts to you specific needs.

### cliMenu

Typing "./taskSplitter -f myTaskFile -c" every time is very tedious. CliMenu fixes this

On launch, you will be prompted to choose the menu you want to use.
There is currently only one menu availible : cliMenu, option 1.

Type 1 and press enter.

You will be prompted with "> "

Here are the 3 specific commands to cliMenu. They do not work in the cli.

- open \[filepath\] : Opens the file at "filepath" (ex: open myTaskFile).

- save : Saves the file but doesn't close it.

- close : Saves and close the file.

- quit/exit : Doesn't save and close the program.

When a file is open, all other commands work the same, but you can ommit the "./taskSplitter -f myTaskFile -c".

Example : 

With the file myTaskFile open in cliMenu :

```> addTask```

will have the same effect in the cli as :

```$ ./taskSplitter -f myTaskFile -c addTask```

***cliMenu also supports command history with up and down arrows***

### Commands

All commands will be shown for use in the cliMenu, but all are applicable in the cli by adding "./taskSplitter -f filepath -c command" at the start.

Also, you can use multiple chained "-c" flags to run multiple commands chained in the cli and cliMenu.

Example :

```> addTask -c reId -c flow```

(Those commands are explained later).

---

#### addTask

The addTask command adds a new task with an id starting at 0 and increments the nextId variable (so that the next task has id 1, the next 2 and so on).

ex :

```> addTask```

Task created with id 0
It can be shortened to "a" :

```> a```

Task created with id 0

---

#### rmTask taskId

rmTask removes the task with id "taskId".
If i want to remove the task with id 0, I will do :

```> rmTask 0```

Or shortened :

```> r 0```

This removes the task everywhere : 

If task 0 is dependent on task 1, removing task 1 will cause 0 to not be dependent on task 1 anymore as it has been removed and thus doesn't exist anymore.

---

#### name task

Change the name of a task (default is " ").

Newlines and empty names are disallowed

ex (without spaces):

```> name 0 mytask```

ex (with spaces use double quotes):

```> name 0 "my task"```

Or shortened : 

```> na 0 "my task"```

---

#### desc task

Change the description of a task (default is " ").

Newlines and empty descriptions are disallowed

ex (without spaces):

```> desc 0 mydescription```

ex (with spaces use double quotes):

```> desc 0 "my description"```

Or shortened : 

```> de 0 "my description"```

---

#### addDep task dep

addDep adds a dependecy to "task" to "dep"
If i want task 0 to be dependent on task 1 I will do :

```> addDep 0 1```

Or shortened :

```> addD 0 1```

---

#### rmDep task dep

rmDep removes a dependecy from "task" to "dep"
If i want to remove the dependency on task 0 for task 1 I will do :

```> rmDep 0 1```

Or shortened :

```> rmD 0 1```

---

#### due taskId \[\[+\]Yd\] \[\[+\]Md\] \[\[+\]Dd\] \[\[+\]hd\] \[\[+\]md\] 

Due sets the due time of a task (when it has to be finished for)

With one command you can set :

- Y : year
- M : month
- D : day
- h : hour
- m : minutes

For a specific time (here september 12th 2026 at 12:08) :
(Time is for now entered in 24h format)

```> due 0 2026Y 9M 3D 12h 8m```

For a partially specific time :
If you do not specify a part of time, if will be set to it's current value (current time).
This will set the due of 0 to the current day, month and year at 12:08 :

```> due 0 12h 8m```

This way, giving no time parts sets the time to current time :

```> due 0```

In a specific amount of time :
You can add a '+' in front of a time part to set it to current time part value + value entered. See the example :
This will set the due of 0 in 1 hour :

```> due 0 +1h```

It works even if the target time is the next month or/and year :
31th december 2026 at 12:00 + 1 day = 1st january 2027 at 12:00

```> due 0 +1D```

---

#### coolPrint taskId

This prints all informations about a task by its task id (taskId) in a human readable format.

Example :

```> coolPrint 0```

```0 [1,2] | taskName PAST DUE
taskDesc
made 11-08-2026 16:23 due 11-08-2022 16:23
```

From left to right top to bottom :

- Id (here 0, inverted if COLORS)
- Dependencies (here 1 and 2)
- Name (here taskName, empty if name is " ")
- "PAST DUE" mention if the due time is older than current time
- Description (here taskDesc, faint if COLORS, empty if description is " ")
- Creation date (DD-MM-YYYY hh:mm)
- Due date (DD-MM-YYYY hh:mm)

If the current time is past the due time, the "PAST DUE" mention will appear with a red background (if the option is enable).

---

#### print taskId

This prints the task with id taskId as printed in the saveFile. This is less practical than the command coolPrint and is more for debugging purposes. You should not use it very often.

```> print 0```

---

#### flow

This is an important command :

It sorts all the tasks from top to bottom :

- from nearest due time to furthest
Then
- Tasks are bubbled up over their own dependencies (this means dependencies of a task are always before the task)

Then it prints all of the tasks in this order using coolPrint, and separating with dashes ('-').

Example :

```> flow```

---

#### flowSort

FlowSort if like flow, but without the printing (just the sorting).

Example :

```> flowSort```

---

#### search yourSearch

This command will coolPrint all the tasks (in flow's style)that contain yourSearch in their name.
If i want a list of all tasks with literal "pizza" in their name, i would do :

```> search pizza```

If i want a list of all tasks with literal "my pizza" in their name (with spaces), i would do :

```> search "my pizza"```

---

#### reId

As time passes using taskSplitter, the next id number may get high and tedious to type in and or remember.
The command reId fixes this, because it will reroll the ids starting from 0, in tasks and in dependencies, and set nextId accordingly so that the ids are continuous again.

If your tasks are : 0, 11, 39, 16

```> reId``` 

(Shortened "re")

Would make them : 0, 1, 2, 3

---

#### clear

Clear just clears the screen without having to exit the program. Your terminal must support ANSI escape characters.

Example :

```clear```

### Tips and infos

Notice :

***Those tips are entirely personal experience, so feel free to agree or not !***

---

#### Make tasks for anything and everything

Sometimes, we tell ourselves to do something right away (so we think it's not necessary to take note of), but it's very easy to get distracted by something else and totally forget. 

Even things we think we are going to remember, we forget for whatever reason !

As adding a task and naming it is very quick, i recommend to add a task for those kind of reminders 

---

#### "Event" tasks

If some task requires you to be somewhere, you can create a task named "Be at Tokyo" for example and have have your task dependent on it :

```FLOW :

0 [1] | Eat sushi
made 11-08-2026 16:36 due 18-08-2026 16:36
_______________________________________________

1 [] | Be at tokyo
made 11-08-2026 16:36 due 18-08-2026 16:36
_______________________________________________
```

And if you need to take luggage for your trip for example you can make a task "Take luggage" and make this task a dependency of "Be at Tokyo".

```FLOW :

0 [1] | Eat sushi
made 11-08-2026 16:36 due 18-08-2026 16:36
_______________________________________________

1 [2] | Be at tokyo
made 11-08-2026 16:36 due 18-08-2026 16:36
_______________________________________________

2 [] | Take luggage
made 11-08-2026 16:38 due 18-08-2026 16:38
_______________________________________________
```

---

#### Due vs dependencies

As seen with the previous tip, the use of dependencies can order automatically the tasks, so i recommand to use due times only when a task *needs* to be done at a specific time, like an appointment or deadlines. The rest should use dependencies as it permits automatic sorting.

---

#### Why no tasks linking to taskLists ?

The whole goal of this project is to have all of the tasks in a single list. Adding cross-file dependencies would be unintitive. Also the flow needs the user to keep in mind the bottom 3 to 5 tasks, to not be overwhelmed, so this defeats the whole purpose if the task is a list of tasks itself as it is harder to keep in mind.

---

#### Origin of the name

My first idea was to have a main task that could be "split" into smaller tasks. This was great, but i found it easier and more versatile to create tasks and add dependencies later. The program still fills the same needs, so i think the name still fits and that's why i decided to keep it.

---

#### Use of AI for making this program

AI was not used a lot for this project. I only used it to find where the bugs i was stuck on for at least 1 week were coming from. I rarely used it for correcting those bugs. I think the main direction this project takes is better with less AI because I can take the time to think about what features I really want to add and what fits the real goal.

---

#### Roadmap

- Multiple task selection (batch use of commands)
- Real TUI with a library such as Ncurses
- Fixing bugs (If you find bugs, I'll be happy to fix them :) )
- Adding installation instructions for more distros and OSes
- Add precompiled binaries in Releases
- Add code documentation
- Optimize sorting algorithm (bubble sort for now :( )

---
