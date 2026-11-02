#include <dirent.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  struct dirent *pDirent;
  DIR *pDir;

  // Check that there is the correct number of arguments
  if (argc != 2) {
    printf("Usage ftree <dirname> \n");
    return 1;
  }

  pDir = opendir(argv[1]);
  if (pDir == NULL) {
    printf("Cannot open the directory '%s' \n", argv[1]);
    return 1;
  }

  while ((pDirent = readdir(pDir)) != NULL) {
    printf("['%s']\n", pDirent->d_name);
  }

  closedir(pDir);
  return 0;
}
