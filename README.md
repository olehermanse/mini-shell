# mini-shell

## Information
**Date:** 2016-08-08<br>
**Author:** olehermanse ( http://www.github.com/olehermanse )<br>
**License:** The MIT License<br>

## Functionality
### Built-in commands
#### Change dir
```
olehermanse@mini-shell 0> pwd
/Users/olehermanse/github/mini-shell
olehermanse@mini-shell 1> cd src
olehermanse@mini-shell 2> pwd
/Users/olehermanse/github/mini-shell/src
```
#### Exit
```
olehermanse@mini-shell 3> exit
Good-bye!
```
### Operators
#### & - Do not wait
Shell does not wait(and hang) until forked child process finishes:
```
olehermanse@mini-shell 0> firefox &
Child process started: [60508].
```
#### | - Pipes
Pipes can be used to redirect input and output from one program to another:
```
olehermanse@mini-shell 0> cat README.md | grep olehermanse | wc -l
      12
```

## Assumptions
A few assumptions about the input was made:
* A single line of input is no more than 255 characters long.
* A single line of input contains no more than 25 space separated words
* A single line of input contains no more than 25 | separated commands

All of these are arbitrarily picked integer constants and can be easily changed.

## Issues
### & and | (Do not wait and pipe)
Not designed or tested for use together.
### Quote strings support
Single or double quoted strings don't behave exactly as expected. See example below.

Standard shell:
```
OHMBA:mini-shell olehermanse$ ec"ho" hello world
hello world
```
Mini-shell:
```
olehermanse@mini-shell 0> ec"ho" hello world
Unrecognized command: 'ec"ho"'
```
### Copy paste and/or backspaces
Some issues can occur when copy pasting and/or deleting characters from a query. As seen below, the seemingly identical input can give different results. (First was pasted, second was typed)

```
olehermanse@mini-shell 16> cat src/external-commands.h | grep bool
Unrecognized command: ' grep'
olehermanse@mini-shell 17> cat src/external-commands.h | grep bool
bool checkExecutable(char* path);
```
## Not Implemented / Improvements
* Shell built-ins like alias, history etc...
* Handle arrow key inputs
* Other redirection operators: <, <<, >, >>, <&, >&
* Custom string parse/split would be better than strtok, specifically for quoted string support.
