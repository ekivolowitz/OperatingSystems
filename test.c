#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #define DEBUG

// fwrite()
// stdout
// fread()
// printf()

int main(int argc, char * argv[]) {
  int count = 91;
  fwrite(&count, sizeof(int), 1, stdout);
  printf("Size of int is %lu", sizeof(int));


  return 0;
}
