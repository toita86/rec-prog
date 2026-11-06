#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLOR_BOLD "\e[1m"
#define COLOR_OFF "\e[m"

void print_level_tabs(int level) {
  for (int i = 0; i < level; i++)
    printf("-");
}

void print_folder_content(char *dir, DIR *pDir, int level, int limit) {
  if (level > limit && limit != -1)
    return;
  struct dirent *pDirent;
  while ((pDirent = readdir(pDir)) != NULL) {
    if (pDirent->d_type == DT_REG) {
      print_level_tabs(level);
      printf(" %s\n", pDirent->d_name);
    }
    if (pDirent->d_type == DT_DIR) {
      if ((strcmp(pDirent->d_name, ".") == 0 ||
           strcmp(pDirent->d_name, "..") == 0))
        continue;
      char *subdir = malloc(strlen(dir) + strlen(pDirent->d_name) + 2);
      // Concatenate directory name
      strcpy(subdir, dir);
      strcat(subdir, "/");
      strcat(subdir, pDirent->d_name);

      DIR *psubDir = opendir(subdir);
      print_level_tabs(level);
      printf(COLOR_BOLD " %s\n" COLOR_OFF, pDirent->d_name);
      print_folder_content(subdir, psubDir, ++level, limit);
      level--;
      free(subdir);
      closedir(psubDir);
    }
  }
  return;
};

int main(int argc, char *argv[]) {
  DIR *pDir;
  int limit = -1;

  // Check that there is the correct number of arguments
  if (argc < 2) {
    printf("Usage ftree <dirname> <depth>(optional) \n");
    return 1;
  }
  if (argc == 3) {
    limit = atoi(argv[2]);
  }

  pDir = opendir(argv[1]);
  if (pDir == NULL) {
    printf("Cannot open the directory '%s' \n", argv[1]);
    return 1;
  }

  printf("-" COLOR_BOLD " .\n" COLOR_OFF);
  print_folder_content(argv[1], pDir, 1, limit);

  closedir(pDir);
  return 0;
}
