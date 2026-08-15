# Custom Linux Shell in C

A custom command-line shell implemented in C.

The project demonstrates core operating-system and systems-programming concepts such as process creation, command execution, pipes, file redirection, file operations, and dynamic memory management.

## Features

- Interactive command-line prompt
- Execute external Linux commands
- Process creation using `fork()`
- Command execution using `execvp()`
- Pipe support using `|`
- Output redirection using `>`
- Append redirection using `>>`
- Current working directory display
- Custom built-in commands
- File manipulation
- Dynamic command parsing

## Built-in Commands

The shell includes support for commands such as:

- `cd` – Change the current working directory
- `echo` – Print text to the terminal
- `cp` – Copy files
- `mv` – Move or rename files
- `delete` – Delete files
- `read` – Display file contents
- `wc -l` – Count lines in a file
- `wc -w` – Count words in a file
- `exit` – Exit the shell

## Pipes

The shell supports piping between commands.

Example:

```bash
ls | wc -l
