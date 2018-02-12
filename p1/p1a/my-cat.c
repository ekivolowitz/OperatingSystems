#include <stdio.h>
#include <stdlib.h>
// #define DEBUG 
#define BUFFER_SIZE 512
int main(int argc, char * argv[]) {
  if(argc == 1) return 0;
  #ifdef DEBUG
    printf("DEBUG> ARGC = %d\n", argc);
    printf("DEBUG> Beginning scan of filenames passed to cat.\n");
  #endif
  for(int i = 1; i < argc; ++i) {
    #ifdef DEBUG
      printf("Opening file: %s\n", argv[i]);
    #endif
    FILE * fp = fopen(argv[i], "r");
    if(fp == NULL) {
      #ifdef DEBUG
        printf("File %s did not open correctly\n", argv[i]);
      #endif
      printf("my-cat: cannot open file\n");
      exit(1);
    }
    else {
      char buffer[BUFFER_SIZE];
      while(fgets(buffer, BUFFER_SIZE, fp) != NULL)
        printf("%s", buffer);
      fclose(fp);
    }
  }

  return 0;
}
