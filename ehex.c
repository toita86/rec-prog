#include <ctype.h>
#include <stdio.h>

/*
A mini hexdump
*/
void print_line(unsigned char buffer[16], int *offset, int *read) {
  printf("%6d | ", *offset += *read);

  for (int i = 0; i < *read; i++) {
    if (i != 0 && i % 4 == 0) {
      printf(" ");
    }
    printf("%2x ", buffer[i]);
  }

  for (int s = *read; s < 16; s++) {
    printf("   ");
  }

  printf(" | ");

  for (int j = 0; j < *read; j++) {
    if (isprint(buffer[j]))
      printf("%c", buffer[j]);
    else
      printf(".");
  }

  printf("\n");
}

void dump_file(FILE *fp) {
  unsigned char buffer[16];
  int offset = -16;
  int read = 0;

  while ((read = fread(buffer, 1, 16, fp)) > 0) {
    print_line(buffer, &offset, &read);
  }
}

int main(int argc, char *argv[]) {
  FILE *fp;

  if (argc < 2) {
    printf("Usage fhex <file_name>");
    return 1;
  }

  fp = fopen(argv[1], "r");
  if (fp == NULL)
    printf("Cannot open the file '%s' \n", argv[1]);
  dump_file(fp);
  fclose(fp);
  return 0;
}
