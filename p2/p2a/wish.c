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
  strcpy(tempInput, string);
  int numArgs = -1; // -1 to offset the program counting as an arg
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

  char ** arguments = malloc(sizeof(char *) * numArgs);

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

void freeDoubleCharArray(char ** array, int numElements) {
  for(int i = 0; i < numElements; ++i) {
    free(array[i]);
  }
}

void handleExit(char * command) {
  findNumArgs(command) == 0 ? exit(0) : errMessage();
}


void handlePath(char * path, char * command) {
  char ** addToPaths = getArguments(command);
  int numberOfArgsInPath = findNumArgs(command);
  #ifdef DEBUG
    for(int j = 0; j < numberOfArgsInPath; j++) {
      printf("Arg %d is: %s\n", j, addToPaths[j]);
    }
  #endif
  for(int pathArg = 0; pathArg < numberOfArgsInPath; pathArg++) {
  char * arg = addToPaths[pathArg];
  char spaceAdd[strlen(arg) + 1];
  strcpy(spaceAdd, " ");
  strcat(spaceAdd, arg);
  strcat(path, spaceAdd);
  #ifdef DEBUG
    printf("spaceAdd is: %s\n", spaceAdd);
    printf("Path is: %s\n", path);
  #endif
  }
  freeDoubleCharArray(addToPaths, numberOfArgsInPath);
}

void handleCD(char * command) {
  if(findNumArgs(command) != 1)  {
    errMessage();
    return;
  }
  char ** arg = getArguments(command);
  int success = chdir(arg[0]);
  free(arg[0]);
  if(success != 0) errMessage();
}

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

          if(strcmp("exit", program) == 0)  handleExit(command);
          else if(strcmp("cd", program) == 0) handleCD(command);
          else if(strcmp("path", program) == 0)handlePath(path, command);
          else {
            // handle everything else
          }
          free(program);
        }
        freeDoubleCharArray(commands, numCommands);
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
