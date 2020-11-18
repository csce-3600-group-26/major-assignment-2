# Major Assignment 2
To run the program:
```
make
./newshell
```
# Team Members
- Brandon Dumas
- Richard Lin
- Ha Nguyen
- Jasmine Wicker

# Organization of the Project
| Feature | Programmer |
| --- | --- |
| ```cd``` | Jasmine Wicker |
| ```exit``` | Richard Lin |
| ```path``` | Ha Nguyen |
| ```myhistory``` | Brandon Dumas |
| Input Redirection | Jasmine Wicker |
| Output Redirection | Brandon Dumas |
| Pipelining | Ha Nguyen |
| Signal Handling and Terminal Control | Richard Lin |
| ```alias``` | Richard Lin |

# Design Overview

# Complete Specification
### ```cd```
Write about how you implemented the ```cd``` command.
### ```exit```
The ```exit``` command terminates the shell by calling ```exit()```.
### ```path```
Write about how you implemented the ```path``` command.
### ```myhistory```
Write about how you implemented the ```myhistory``` command.
### Input Redirection
Write about how you implemented Input Redirection.
### Output Redirection
Write about how you implemented Output Redirection.
### Pipelining
Write about how you implemented Pipelining.
### Signal Handling and Terminal Control
The shell ignores the following signals: SIGINT, SIGQUIT, SIGTSTP, SIGTTIN, SIGTTOU. When a command is executed, after ```fork()``` is called, the ignored signals will be reset to their default actions for the child process. Additionally, all commands in the same pipeline will share the same process group ID and be placed in the foreground upon execution.
### ```alias```
The ```alias``` command uses a null-terminated array to store aliases. Each alias is an instance of ```struct alias```. ```struct alias``` contains 2 member variables which are strings. One string contains the alias's name and the other string contains a command that can be parsed and executed by the shell. An instance of ```struct alias``` can be created by calling ```new_alias()``` and be deleted by calling ```delete_alias()```.

Four functions are defined to implement the functionality of the ```alias``` command: ```alias_add()```, ```alias_remove()```, ```alias_clear()```, and ```alias_print()```.

```alias_add()``` first checks whether the alias exists. If true, the old alias is deleted and the new alias is inserted at the position of the old alias. If false, the alias is appended to the end of the array. ```alias_add()``` is called when the 1st argument matches the regex for assigning aliases.

```alias_remove()``` first checks whether the alias exists. If true, a new array is allocated and the aliases that are positioned before and after the target alias are copied to the new array. Then, the target alias is deleted and the memory from the old array is freed. If false, an error message is printed to standard error. ```alias_remove()``` is called when ```alias -r alias_name``` is entered in the shell.

```alias_clear()``` deletes all of the aliases in the array and then shrinks the array to size 1, where index 0 points to ```NULL```. ```alias_clear()``` is called when ```alias -c``` is entered in the shell.

```alias_print()``` prints all of the aliases by iterating through the array and printing the name and command of the alias to standard output. ```alias_print()``` is called when no arguments are passed to the ```alias``` command.

# Known Bugs or Problems
