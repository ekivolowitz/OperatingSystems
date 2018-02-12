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

char * tokenize(char * string, const char * delim) {
  return strtok(string, delim);
}

// Expecting modification of string
char ** getCommands(char * string) {
  char * delim = "&";
  char ** commands = malloc(sizeof(char *) * strlen(string));
  for(int i = 0; i < strlen(string); i++) {
    commands[i] = NULL;
  }
  int count = 0;
  char 
  * splitOnAnd = tokenize(string, delim);
  while(splitOnAnd != NULL) {
    commands[count] = splitOnAnd;
    count++;
    splitOnAnd = tokenize(NULL, delim);
  }
  return commands;
}


int findNumArgs(const char * string) {
  // Create a local copy of the string
  char * whitespace = " \t\n";
  char tempInput[strlen(string)];
  int numArgs = -1; // -1 to offset the program counting as an arg
  strcpy(tempInput, string);
  char * splitOnWhiteSpace = tokenize(tempInput, whitespace);
  while(splitOnWhiteSpace != NULL) {
    numArgs++;
    splitOnWhiteSpace = tokenize(NULL, whitespace);
  }
  return numArgs;
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
        // char * space = " ";
        // char * exitWord = "exit";
        // char * exitWordNewLine = "exit\n";
        // char * cdWord = "cd";
        // char * cdWordNewLine = "cd\n";
        // char * pathWord = "path";
        // char * pathWordNewLine = "path\n";

        size_t length = 0;
        ssize_t read;
        read = getline(&input, &length, stdin);
        if(read == -1) { 
          printf("Error reading from stdin.\n");
          exit(1);
        }
        char tempInput[length];
        strcpy(tempInput, input);
        // char * splitOnSpace = tokenizeSpace(tempInput, space);
        // char ** commands = getCommands(tempInput);
        // printf("%s", commands[0]);
        // free(commands);


        // while(splitOnSpace != NULL) {
          // test for exit / cd / path
          
        //   // Tests if cd [args] exit [args] path [args] was entered, including the words themselves with the newline character.
        //   if(   
        //       strcmp(exitWordNewLine, splitOnSpace) == 0 || 
        //       strcmp(exitWord, splitOnSpace) == 0 ||
        //       strcmp(cdWord, splitOnSpace) == 0 ||
        //       strcmp(cdWordNewLine, splitOnSpace) == 0 ||
        //       strcmp(pathWord, splitOnSpace) == 0 || 
        //       strcmp(pathWordNewLine, splitOnSpace) == 0
        //   ){
        //     // Exit check. Checks that either exit asfasdf or exit\n was entered but excludes whitespace 
        //     // 
        //     if(strcmp(exitWord, splitOnSpace) == 0 || strcmp(exitWordNewLine, splitOnSpace) == 0) {
        //       //In the event that exit has params sent to it, error
        //       if((splitOnSpace = tokenizeSpace(NULL, space)) != NULL && strcmp(splitOnSpace, "\n") != 0) {
        //         errMessage();
        //       } else exit(0);
        //     }
        //     else if(strcmp(cdWord, splitOnSpace) == 0) {
        //       // handle cd
        //     }/* end handle cd */ else {
        //       // handle path
        //       char temp[length];
        //       strcpy(temp, input);
        //       char * tempSplitOnSpace = tokenizeSpace(temp, space);
        //       while(tempSplitOnSpace != NULL) {
        //         if(strcmp(pathWord, tempSplitOnSpace) != 0) {
        //           char addToPath[strlen(tempSplitOnSpace) + 1];
        //           addToPath[0] = ' ';
        //           strncat(addToPath, tempSplitOnSpace, strlen(tempSplitOnSpace) - 1);
        //           printf("addToPath is %s\n", addToPath);
        //           strcat(path, addToPath);

        //         }
        //         printf("Path is: %s\n", path);
        //         tempSplitOnSpace = tokenizeSpace(NULL, space);
        //       }

        //     } // end handle path
        //   } else {
        //     printf("Not recognized\n");
        //   }
        //   splitOnSpace = tokenizeSpace(NULL, space);
        // }
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
