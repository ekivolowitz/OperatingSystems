#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define DEBUG

// Write a function to check how many args are in a statement. There will be a lot of arg
// number checking. 

void errMessage() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
  exit(1);
}

char * tokenizeSpace(char * string, const char * delim) {
  return strtok(string, delim);
}



int main(int argc, char * argv[]) {
  char path[1024];
  char * startingPath = "/bin";
  strncpy(path, startingPath, strlen(startingPath));
  printf("length of path: %lu\n", strlen(path));
  printf("Starting path is: %s\n",path);
  switch(argc){
    case 1: ;
      // read from stdin
      while(1) {
        printf("wish> ");
        char * input = NULL;
        char * space = " ";
        char * exitWord = "exit";
        char * exitWordNewLine = "exit\n";
        char * cdWord = "cd";
        char * cdWordNewLine = "cd\n";
        char * pathWord = "path";
        char * pathWordNewLine = "path\n";

        size_t length = 0;
        ssize_t read;
        read = getline(&input, &length, stdin);
        if(read == -1) { 
          printf("Error reading from stdin.\n");
          exit(11);
        }
        char tempInput[length];
        strcpy(tempInput, input);

        char * splitOnSpace = tokenizeSpace(tempInput, space);
        while(splitOnSpace != NULL) {
          // test for exit / cd / path
          
          // Tests if cd [args] exit [args] path [args] was entered, including the words themselves with the newline character.
          if(   
              strcmp(exitWordNewLine, splitOnSpace) == 0 || 
              strcmp(exitWord, splitOnSpace) == 0 ||
              strcmp(cdWord, splitOnSpace) == 0 ||
              strcmp(cdWordNewLine, splitOnSpace) == 0 ||
              strcmp(pathWord, splitOnSpace) == 0 || 
              strcmp(pathWordNewLine, splitOnSpace) == 0
          ){
            // Exit check. Checks that either exit asfasdf or exit\n was entered but excludes whitespace 
            // 
            if(strcmp(exitWord, splitOnSpace) == 0 || strcmp(exitWordNewLine, splitOnSpace) == 0) {
              //In the event that exit has params sent to it, error
              if((splitOnSpace = tokenizeSpace(NULL, space)) != NULL && strcmp(splitOnSpace, "\n") != 0) {
                errMessage();
              } else exit(0);
            }
            else if(strcmp(cdWord, splitOnSpace) == 0) {
              // handle cd
            }/* end handle cd */ else {
              // handle path
              char temp[length];
              strcpy(temp, input);
              char * tempSplitOnSpace = tokenizeSpace(temp, space);
              while(tempSplitOnSpace != NULL) {
                if(strcmp(pathWord, tempSplitOnSpace) != 0) {
                  char addToPath[strlen(tempSplitOnSpace) + 1];
                  addToPath[0] = ' ';
                  strncat(addToPath, tempSplitOnSpace, strlen(tempSplitOnSpace) - 1);
                  printf("addToPath is %s\n", addToPath);
                  strcat(path, addToPath);

                }
                  
                printf("Path is: %s\n", path);
                tempSplitOnSpace = tokenizeSpace(NULL, space);
              }

            } // end handle path
          } else {
            printf("Not recognized\n");
          }
          splitOnSpace = tokenizeSpace(NULL, space);
        }
        // split by spaces and search for exit, cd, and path


      }
      break;


    case 2:
      // read from argv[1]
      break;

    default:
      #ifdef DEBUG
        printf("Did not start up wish correctly. Entered too many arguments.\n"); 
      #endif
      errMessage();
      
  }
  return 0;
}
