#include <dirent.h>
#include <stdio.h>

void print_folder_content(DIR *pDir) {
  struct dirent *pDirent;
  while ((pDirent = readdir(pDir)) != NULL) {
    printf("['%s']\n", pDirent->d_name);
  }
  return;
};

int main(int argc, char *argv[]) {
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

  print_folder_content(pDir);

  closedir(pDir);
  return 0;
}
