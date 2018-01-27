#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define DEBUG

int main(int argc, char * argv[]) {
  

  #ifdef DEBUG
    for(int i = 0; i < argc; ++i) {
      printf("DEBUG> argv[%d]: %s\n", i, argv[i]);
    }
  #endif

  // Case that no file is passed to the call.
  if(argc == 1) {
    #ifdef DEBUG
      printf("DEBUG> No arguments were used - must call with at least one argument.\n");
    #endif
    printf("my-grep: searchterm [file ...]\n");
    exit(1);
  }
  // Case that a search term was sent but not a file
  // Read from STDIN
  else if (argc == 2) {
    #ifdef DEBUG
      printf("DEBUG> One argument used. Reading from STDIN.\n");
    #endif
    char * buffer;
    char * searchTerm = argv[1];
    char * search;
    size_t len = 0;
    ssize_t read;

    #ifdef DEBUG
      printf("DEBUG> Search term is: %s\n", searchTerm);
    #endif
    while((read = getline(&buffer, &len, stdin) != -1)) 
      if((search = strstr(buffer, searchTerm)) != NULL) printf("%s", buffer);
  }
  else {
    // Case that one or more files have been sent to my-grep
    // for loop from 2 to N where N - 2 is the number of files
    #ifdef DEBUG
      printf("DEBUG> Two or more arguments entered.\n");
    #endif
    
    char * searchTerm = argv[1];
    char * search;
    size_t len = 0;
    ssize_t read;
    FILE * fp;
    char * buffer;
    #ifdef DEBUG
      printf("DEBUG> Search term is: %s\n", searchTerm);
      if(!strcmp(searchTerm, "")) printf("DEBUG> Search term is empty string\n");
    #endif

    for(int i = 2; i < argc; ++i) {
      #ifdef DEBUG
        printf("DEBUG> Opening file: %s\n", argv[i]);
      #endif
      fp = fopen(argv[i], "r");
      if(fp == NULL) {
        #ifdef DEBUG
          printf("DEBUG> File %s did not open correctly.\n", argv[i]);
        #endif
        printf("my-grep: cannot open file\n");
        exit(1);
      
      } else {
        
        #ifdef DEBUG
          printf("DEBUG> File %s opened correctly\n", argv[i]);
        #endif
        while((read = getline(&buffer, &len, fp) != -1)) {
          if((search = strstr(buffer, searchTerm)) != NULL)
            printf("%s", buffer);
        }
        fclose(fp);
      }
    }
  }
  return 0;
}
