#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DEBUG

void unzip(FILE * fp) {
    int i = 0;
    char c = '_';
    #ifdef DEBUG
      printf("DEBUG> Entering while loop.\n");
    #endif
    while(fread(&i, sizeof(int), 1, fp) != 0) {
      fread(&c, sizeof(char), 1, fp);
      #ifdef DEBUG
        printf("DEBUG> The value of i is %d\n", i);
        printf("DEBUG> The value of c is %c\n", c);
      #endif
      for(int _ = 0; _ < i; _++) {
        printf("%c", c);
      }
    }
}


int main(int argc, char * argv[]) {
  if(argc == 1) {
    printf("my-unzip: file1 [file2 ...]\n");
    exit(1);
  }
  else if(argc >= 2) {
    for(int index = 1; index < argc; index++) {
      FILE * fp = fopen(argv[index], "r");
      if(fp == NULL) {
        printf("my-unzip: file1 [file2 ...]\n");
        exit(1);
      }
      unzip(fp);
      fclose(fp);
    }
  }


  return 0;
}
