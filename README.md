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
The shell first determines which mode to use based on the number of command-line arguments. If 1 argument is passed, the
shell will enter batch mode. If 0 arguments are passed, the shell will enter interactive mode. Otherwise, the shell will
print an error.

In interactive mode, the shell will print a prompt and wait for a line of input to be entered. In batch mode, the shell
will read a line from the script. After the shell gets a line of input, the shell will add it to the history and pass it
to the parser.

The parser is responsible for deserializing lines of input into statement objects. Each statement object stores a
reference to a command object and a reference to another statement object. The command object stores information such as
the name of the command to execute, a list of command-line arguments, the file path for redirecting input, the file path
for redirecting output, and the command to pipe output to. The reference to a statement object is used to chain multiple
statements for the shell to execute.

The shell uses a context-free grammar to help parse each line of input. If the line of input contains a statement that
is not in the language of the context-free grammar, then the shell will print a syntax error.

After the line of input has been parsed, the shell will use the information in the statement object to execute commands.
```
Let each shell statement be a member of the language of G, such that G is a context-free grammar.
G = (V, T, R, S)
V = { [STATEMENT], [COMMAND], [ARGS], [ALIAS_ARGS], [EXT_ARGS], [PIPE] }
T = { <whitespace>, <built_in>, <alias_cmd>, <alias>, <external>, <argument>, <redirection>, <file_path>, <pipe>, <end> }
R = V × (V ∪ T)*
S = [STATEMENT]

*[STATEMENT]  ->  <whitespace>[COMMAND]<whitespace><end> | <whitespace>[COMMAND]<end> | <whitespace><end> | [COMMAND]<whitespace><end> | [COMMAND]<end> | <end>
[COMMAND]     ->  <built_in>[ARGS] | <alias_cmd>[ALIAS_ARGS] | <external>[EXT_ARGS]
[ARGS]        ->  <whitespace><argument>[ARGS] | ε
[ALIAS_ARGS]  ->  <whitespace><alias> | [ARGS]
[EXT_ARGS]    ->  <whitespace><argument>[EXT_ARGS] | <whitespace><redirection><whitespace><file_path>[EXT_ARGS] | <whitespace>[PIPE] | ε
[PIPE]        ->  <pipe><whitespace><external>[EXT_ARGS]

Let the alphabet Σ be the set of printable ASCII characters (0x20-0x7E), newline (0x0A), and horizontal tab (0x09).
<UPRCS> = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" }
<LWRCS> = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z" }
<LTR> = <UPRCS> ∪ <LWRCS>
<NUM> = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" }
<LTRNUM> = <LTR> ∪ <NUM>

Let each terminal t be a member of a formal language over the alphabet Σ, such that t ∈ T.
<whitespace> ∈ { x^n | x ∈ {"\t", " "}, n >= 1 }
<built_in> ∈ { "cd", "exit", "path", "myhistory" }
<alias_cmd> ∈ { "alias" }
<alias> ∈ { xy*"='"z*"'" | x ∈ <LTR> ∪ {"_"}, y ∈ <LTRNUM> ∪ {"_"}, z ∈ Σ \ {"'", ";"} }
<external> ∈ { ((x"/"^n)∪"/")^nyz*("/"yz*)* | x ∈ {"~", ".", ".."}, y ∈ <LTRNUM> ∪ {"_"}, z ∈ <LTRNUM> ∪ {".", "_", "-"}, n ∈ {0, 1} } \ { "cd", "exit", "path", "myhistory", "alias" }
<argument> ∈ { x^n | x ∈ Σ \ {"\t", " ", ";", "\n", "<", ">", "|", "'", "\"", "\\"}, n >= 1 }
<redirection> ∈ { "<", ">" }
<file_path> ∈ { ((x"/"^n)∪"/")^nyz*("/"yz*)* | x ∈ {"~", ".", ".."}, y ∈ <LTRNUM> ∪ {"_"}, z ∈ <LTRNUM> ∪ {".", "_", "-"}, n ∈ {0, 1} }
<pipe> ∈ { "|" }
<end> ∈ { x∪ε∪";"y*"\n" | x ∈ {";", "\n"}, y ∈ {"\t", " "} }
```

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
