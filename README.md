# Minishell - 42 school project

## Project Overview
**Minishell** 

Minishell is a simple shell built in C as a pair project at Hive Helsinki. Our minishell implements input parsing and command execution with pipes, closely mimicking the core behavior of `bash`.

---
## Features
```
 - Command piping
 - Environment variables
 - Redirections              <  >  >>
       - Heredocs                  << 
 - Built-in functions        echo, cd, pwd, export, unset, env, exit
 - Signal handling           ctrl-C, ctrl-D, ctrl-\       
```  
---
## Build Instructions

```
make             # Build minishell
./minishell      # Launch program without additional arguments
