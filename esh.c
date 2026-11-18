#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define ESH_RL_BUFSIZE 1024
#define ESH_TOK_BUFSIZE 64
#define ESH_TOK_DELIM " \t\r\n\a"

void esh_loop(void);

// Input management rading line and the relative arguments
char *esh_read_line(void);
char **esh_split_line(char *line);

// Core of the shell fork the parent and
// then exec to subtitute with the program we want to launch
int esh_launch(char **args);
int esh_execute(char **args);

// Some builtins are needed a program that changes
// dir cannot execute with esh_launch otherwise only the
// children will change the dir
int esh_builtin_cd(char **args);
int esh_builtin_help(char **args);
int esh_builtin_exit(char **args);
int esh_builtin_thankyou(char **args);

// List of builtin commands, followed by their corresponding functions.
char *builtin_str[] = {"ecd", "ehelp", "exit", "ethankyou"};

int (*builtin_func[])(char **) = {&esh_builtin_cd, &esh_builtin_help,
                                  &esh_builtin_exit, &esh_builtin_thankyou};

int esh_num_builtins() { return sizeof(builtin_str) / sizeof(char *); }

int main(int argc, char **argv) {
  // command loop
  esh_loop();

  return EXIT_SUCCESS;
}

void esh_loop(void) {
  char *line;
  char **args;
  int status;

  do {
    printf("e: ");
    line = esh_read_line();      // reading the command
    args = esh_split_line(line); // parsing program and args
    status = esh_execute(args);  // execute the command

    free(line);
    free(args);
  } while (status);
}

char *esh_read_line(void) {
  int bufsize = ESH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "esh: allocation error sorry sir\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // the fun part begins
    c = getchar();

    // if hit EOF, replace with null and return
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    if (position > bufsize) {
      bufsize += ESH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "esh: reallocation error sorry sir\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char **esh_split_line(char *line) {
  int bufsize = ESH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char *));
  char *token;

  if (!tokens) {
    fprintf(stderr, "esh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, ESH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += ESH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      if (!tokens) {
        fprintf(stderr, "esh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, ESH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

int esh_launch(char **args) {
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // we are in the children
    if (execvp(args[0], args) == -1) {
      perror("esh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // error in the forking process
    perror("esh");
  } else {
    // parent process waits for the children
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

int esh_execute(char **args) {
  int i;

  if (args[0] == NULL) {
    // oopsie there is an empty command
    return 1;
  }

  for (i = 0; i < esh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return esh_launch(args);
}

// Builtin function implementations.
int esh_builtin_cd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "esh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("esh");
    }
  }
  return 1;
}

int esh_builtin_help(char **args) {
  int i;
  printf("Eduard Brahas's ESH\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < esh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int esh_builtin_thankyou(char **args) {

  printf("This was a really fun to do by hand project in the time of "
         "\"<Insert_model_name> do this make no mistakes\" \n");
  printf("Thank you Stephen Brennan \n");
  printf("https://brennan.io/2015/01/16/write-a-shell-in-c/\n");

  return 1;
}

int esh_builtin_exit(char **args) { return 0; }
