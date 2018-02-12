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
}

char * tokenize(char * string, const char * delim) {
  return strtok(string, delim);
}

// Expecting modification of string
char ** getCommands(char * string, int * numCommands) {
  char * delim = "&";
  char ** commands = malloc(sizeof(char *) * strlen(string));
  for(int i = 0; i < strlen(string); i++) {
    commands[i] = NULL;
  }
  int count = 0;
  char * splitOnAnd = tokenize(string, delim);
  while(splitOnAnd != NULL) {
    char * string = malloc(sizeof(char) * strlen(splitOnAnd));
    strcpy(string, splitOnAnd);
    commands[count] = string;
    count++;
    splitOnAnd = tokenize(NULL, delim);
  }
  *numCommands = count;
  return commands;
}

char * getProgName(char * command) {
  char * delim = " \n\t";
  char tempInput[strlen(command)];
  strcpy(tempInput, command);
  char * splitOnWhiteSpace = tokenize(tempInput, delim);
  char * temp = malloc(sizeof(char) * strlen(splitOnWhiteSpace));
  strcpy(temp, splitOnWhiteSpace);
  return temp; 
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

char ** getArguments(char * command) {
  char * delim = " \n\t";
  char tempInput[strlen(command)];
  strcpy(tempInput, command);
  
  int numArgs = findNumArgs(command);

  char ** arguments = malloc(sizeof(char) * numArgs);

  // This should get rid of the command name.
  tokenize(tempInput, delim);
  char * arg = tokenize(NULL, delim);
  for(int i = 0; i < numArgs; ++i) {
    char * string = malloc(sizeof(char) * strlen(arg));
    strcpy(string, arg);
    arguments[i] = string;
    arg = tokenize(NULL, delim);
  }
  return arguments;
}

int freeDoubleCharArray()


int main(int argc, char * argv[]) {
  char path[1024];
  char * startingPath = "/bin";
  strncpy(path, startingPath, strlen(startingPath));
  #ifdef DEBUG
    printf("length of path: %lu\n", strlen(path));
    printf("Starting path is: %s\n",path);
  #endif
  switch(argc){
    case 1: ;
      // read from stdin
      while(1) {

        printf("wish> ");
        char * input = NULL;
        char * exitWord = "exit";
        char * cdWord = "cd";
        char * pathWord = "path";

        size_t length = 0;
        ssize_t read;
        read = getline(&input, &length, stdin);

        if(read == -1) { 
          #ifdef DEBUG
            printf("Error reading from stdin.\n");
          #endif
          exit(1);
        }
        char tempInput[length];
        strcpy(tempInput, input);

        int numCommands = 0;
        char ** commands = getCommands(tempInput, &numCommands);

        for(int i = 0; i < numCommands; ++i) {
          char command[strlen(commands[i])];
          strcpy(command, commands[i]);
          char * program = getProgName(command);
          char ** args = getArguments(command);

          for(int j = 0; j < findNumArgs(command); j++) {
            printf(" %s", args[j]);
          }
          printf("\n");
          #ifdef DEBUG
            printf("Program is: %s\n", program);
          #endif
          
          if(strcmp(exitWord, program) == 0) findNumArgs(command) == 0 ? exit(0) : errMessage();
          else if(strcmp(cdWord, program) == 0) { // handle the case that cd was entered
            // cd 
          } else if(strcmp(pathWord, program) == 0) { // handle the case that path was entered 
            // path
            

          } else {
            // handle everything else
          }
          // free(args);
          free(program);
        }
        // free(commands);
        for(int i = 0; i < numCommands; i++) {
          free(commands[i]);
        }
        free(commands);
        free(input);
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
  exit(0);
  return 0;
}
