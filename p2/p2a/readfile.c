#include <stdio.h>

int main() {
  char * line = NULL;
  FILE * fp = fopen("test.txt", "r");
  size_t size = 0;
  size_t read = 0;
  while ((read = getline(&line, &size, fp)) != -1) {
    printf("%s", line);
  }


  return 0;
}
