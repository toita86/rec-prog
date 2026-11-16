#include <stdio.h>
#include <stdlib.h>

#define ESH_RL_BUFSIZE 1024

void esh_loop(void);
char *esh_read_line(void);

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
    printf("e:");
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
