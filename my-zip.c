#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DEBUG

// fwrite()
// stdout
// fread()
// printf()

int main(int argc, char * argv[]) {
  if(argc == 1) {
    printf("my-zip: file1 [file2 ...]\n");
    exit(1);
  }
 
  // Note that the redirection does not count as an argument. 
  #ifdef DEBUG
    printf("DEBUG> Number of arguments: %d\n", argc);
    for(int i = 0; i < argc; ++i) {
      printf("DEBUG> Argument %d: %s\n", i, argv[i]);
    }
  #endif

  for(int i = 1; i < argc; ++i) {
    FILE * fp = fopen(argv[i], "r");
    if(fp == NULL) {
      printf("cannot open file\n");
      exit(1);
    }

    char * buffer;
    size_t len = 0;
    ssize_t read;

    while((read = getline(&buffer, &len, fp)) != -1) {
      #ifdef DEBUG
        printf("DEBUG> Running while loop. Verifies that getline is working properly.\n");
      #endif
      int count = 0;
      int runner = 0;
      int curr = 0;
      size_t lengthOfWord = strlen(buffer);
      
      while(runner < (int)lengthOfWord) {
        while(buffer[curr] == buffer[runner]) {
          if(runner == lengthOfWord) break;
          count++;
          runner++;
        }
        #ifdef DEBUG
          printf("DEBUG> Output should be: %d%c\n", count, buffer[curr]);
        #endif
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&buffer[curr], sizeof(char), 1, stdout);
        curr = runner;
        count = 0;
      }
    }
    fclose(fp);
  }




  return 0;
}
