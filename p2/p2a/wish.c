#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
// #define DEBUG

// Write a function to check how many args are in a statement. There will be a lot of arg
// number checking. 

void errMessage() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
}

void freeDoubleCharArray(char ** array, int numElements) {
  for(int i = 0; i < numElements; ++i) {
    free(array[i]);
  }
}


char * tokenize(char * string, const char * delim) {
  return strtok(string, delim);
}

int findNumWhiteSpaces(char * string) {
  char tempString[strlen(string) + 1];
  strcpy(tempString, string);
  int count = 0;
  char * delim = " ";
  char * s = tokenize(tempString, delim);
  while(s != NULL){
    count++;
    s = tokenize(NULL, delim);
  }
  return count;
}

int hasRedirect(char * command) {
  char tempCommand[strlen(command) + 1];
  strcpy(tempCommand, command);
  return strchr(tempCommand, '>') == NULL ? 0 : 1;
}


int hasOneRedirect(char * command) {
  char tempCommand[strlen(command) + 1];
  strcpy(tempCommand, command);
  return strchr(tempCommand, '>') == strrchr(tempCommand, '>') && strchr(tempCommand, '>') != NULL ? 1 : 0;
}

char * getPreRedirect(char * command) {
  char tempCommand[strlen(command) + 1];
  strcpy(tempCommand, command);
  char * delim = ">";
  char * pre = tokenize(tempCommand, delim);
  char * ret = malloc(sizeof(char) * strlen(pre) + 1);
  strcpy(ret, pre);
  return ret;
}

char * getPostRedirect(char * command) {
  char tempCommand[strlen(command) + 1];
  strcpy(tempCommand, command);
  char * delim = ">";
  char * post = tokenize(tempCommand, delim);
  post = tokenize(NULL, delim);
  char * ret = malloc(sizeof(char) * strlen(post) + 1);
  strcpy(ret, post);
  if(ret[strlen(post)] != '\0') printf("err\n");
  return ret;
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
    char * string = malloc(sizeof(char) * strlen(splitOnAnd) + 1);
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
  char * temp = malloc(sizeof(char) * strlen(splitOnWhiteSpace) + 1);
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

int findNumFiles(char * files) {
  char * whitespace = " \t\n";
  char tempInput[strlen(files) + 1];
  strcpy(tempInput, files);
  int count = 0;
  char * splitOnWhiteSpace = tokenize(tempInput, whitespace);
  while(splitOnWhiteSpace != NULL) {
    count++;
    splitOnWhiteSpace = tokenize(NULL, whitespace);
  }
  return count;
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
    char * string = malloc(sizeof(char) * strlen(arg) + 1);
    strcpy(string, arg);
    arguments[i] = string;
    arg = tokenize(NULL, delim);
  }
  return arguments;
}


char * handlePath(char * path, char * command) {

  // Gets all of the arguments as a list of strings. Remember to free this shit. 
  int numberOfArgsInPath = findNumArgs(command);
  if(numberOfArgsInPath == 0) {
    char tempPath[strlen(path) + 1];
    strcpy(tempPath, path);
    printf("tempPath is: %s\n", tempPath);
    free(path);
    char * newPath = malloc(sizeof(char) * strlen(tempPath) + 1);
    strcpy(newPath, tempPath);
    return newPath;
  }
  char ** addToPaths = getArguments(command);

  size_t sizeOfNewPaths = 0;

  // have to add 2 at the end. One because strlen excludes the null terminator, one because we have to prepend
  // a space to the string. 
  for(int pathArg = 0; pathArg < numberOfArgsInPath; pathArg++) sizeOfNewPaths += strlen(addToPaths[pathArg]) + 2;
  
  // + 1 because strlen does not count the null terminator
  size_t totalNewPathSize = strlen(path) + sizeOfNewPaths + 1;
  char tempPath[totalNewPathSize];
  strcpy(tempPath, path);
  free(path);

  for(int pathArg = 0; pathArg < numberOfArgsInPath; pathArg++) {
    strcat(tempPath, " ");
    strcat(tempPath, addToPaths[pathArg]);
  }

  char * newPath = malloc(sizeof(char) * totalNewPathSize);
  strcpy(newPath, tempPath);
  freeDoubleCharArray(addToPaths, findNumArgs(command));
  free(addToPaths);
  return newPath;
}

void handleCD(char * command) {
  if(findNumArgs(command) != 1)  {
    errMessage();
    return;
  }
  char ** arg = getArguments(command);
  int success = chdir(arg[0]);
  freeDoubleCharArray(arg, 1);
  free(arg);
  if(success != 0) errMessage();
}

int getNumPaths(char * path) {
  char tempPath[strlen(path) + 1];
  for(int i = 0; i < strlen(path); i++) tempPath[i] = '\0';
  char * delim = " ";
  int count = 0;
  strcpy(tempPath, path);
  char * splitOnWhiteSpace = tokenize(tempPath, delim);
  while(splitOnWhiteSpace != NULL) {
    count++;
    splitOnWhiteSpace = tokenize(NULL, delim);
  }
  return count;
}


char ** getPaths(char * path) {
  char tempPath[strlen(path) + 1];
  int numPaths = getNumPaths(path);
  char * delim = " ";
  if(numPaths <= 0) {
    #ifdef DEBUG
      printf("getPaths is borked. Less than or equal to 0 paths.\n");
    #endif
    exit(1);
  }
  strcpy(tempPath, path);
  char ** paths = malloc(sizeof(char *) * numPaths);
  char * splitOnWhiteSpace = tokenize(tempPath, delim);
  int count = 0;
  while(splitOnWhiteSpace != NULL) {
    char * string = malloc(sizeof(char) * (strlen(splitOnWhiteSpace) + 1));
    strcpy(string, splitOnWhiteSpace);
    paths[count] = string;
    count++;
    splitOnWhiteSpace = tokenize(NULL, delim);
  }

  #ifdef DEBUG
    for(int i = 0; i < numPaths; i++) {
      printf("Path[%d]: %s\n", i, paths[i]);
    }
  #endif

  return paths;
}


char ** getTokenizedCommandInput(char * command) {
  char tempCommand[strlen(command) + 1];
  strcpy(tempCommand, command);
  char * delim = " \n\t";

  // + 1 item for command name + 1 item for appended NULL
  int numItems = findNumArgs(command) + 2;
  int count = 0;
  char ** items = malloc(sizeof(char *) * numItems);  
  char * item = tokenize(tempCommand, delim);
  while(item != NULL) {
    char * temp = malloc(strlen(item) + 1);
    strcpy(temp, item);
    items[count] = temp;
    count++;
    item = tokenize(NULL, delim);
  }
  items[count] = NULL;

  #ifdef DEBUG
    for(int i = 0; i < numItems; i++) {
      printf("The %d index of items is %s\n", i, items[i]);
    }
  #endif
  return items;
}

void executeCommand(char * command, char * path) {
  
  char tempPath[strlen(path) + 1];
  strcpy(tempPath, path);
  int numPaths = getNumPaths(path);
  char ** paths = getPaths(tempPath);
  char ** tok = getTokenizedCommandInput(command);
  char * fullCommand;
  
  for(int i = 0; i < numPaths; i++) {
    char fullPath[strlen(tok[0]) + strlen(paths[i]) + 2];
    strcpy(fullPath, paths[i]);
    strcat(fullPath, "/");
    strcat(fullPath, tok[0]);
    if(access(fullPath, X_OK) == 0) {
      fullCommand = malloc(sizeof(char) * strlen(fullPath) + 1);
      strcpy(fullCommand, fullPath);
    } else {
      fullCommand = NULL;
    }
  }

  if(fullCommand == NULL) {
    errMessage();
    freeDoubleCharArray(tok, findNumArgs(command) + 2);
    free(tok);
    freeDoubleCharArray(paths, numPaths);
    free(paths);

    return;
  } 

  if(hasRedirect(command)) {
    if(hasOneRedirect(command)) {
      char * preRedirect = getPreRedirect(command);
      char * postRedirect = getPostRedirect(command);
      #ifdef DEBUG
        printf("Full command is %s\n", fullCommand);
        printf("preRedirect is: %s\n", preRedirect);
        printf("postRedirect is: %s", postRedirect);
      #endif
      char ** preRedirectTok = getTokenizedCommandInput(preRedirect);
      int numAfterRedirect = findNumFiles(postRedirect);
      if(numAfterRedirect != 1) {
        errMessage();
      } else {
        int rc = fork();
        if(rc < 0) {
          exit(1);
        } else if(rc == 0) {
          char * delim = " \t\n";
          char * cleanedPostRedirect = tokenize(postRedirect, delim);
          freopen(cleanedPostRedirect, "w", stdout);
          execv(fullCommand, preRedirectTok);
        } else {
          wait(NULL);
        }
      }
      freeDoubleCharArray(preRedirectTok, findNumWhiteSpaces(preRedirect));
      free(preRedirect);
      free(postRedirect);
      free(preRedirectTok);

    } else {
      errMessage();
    }
  } else {
    int rc = fork();
    if(rc < 0) {
      fprintf(stderr, "fork failed\n");
      exit(1);
    } else if (rc == 0) {
      execv(fullCommand, tok);
    } else wait(NULL);
  }
  free(fullCommand);
  freeDoubleCharArray(tok, findNumArgs(command) + 2);
  free(tok);
  freeDoubleCharArray(paths, numPaths);
  free(paths);

}

int main(int argc, char * argv[]) {
  char * path = malloc(sizeof(char) * 5);
  char * startingPath = "/bin";
  strncpy(path, startingPath, strlen(startingPath) + 1);

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
          exit(1);
        }
        char tempInput[length];
        strcpy(tempInput, input);

        int numCommands = 0;
        char ** commands = getCommands(tempInput, &numCommands);

        for(int i = 0; i < numCommands; ++i) {
          char command[strlen(commands[i]) + 1];
          strcpy(command, commands[i]);
          if(strcmp("\n", command) == 0) break;
          char * program = getProgName(command);
          if(strcmp("exit", program) == 0) {
            if(findNumArgs(command) == 0) {
              freeDoubleCharArray(commands, numCommands);
              free(commands);
              free(program);
              free(input);
              free(path);
              exit(0);
            } else errMessage();
            
          } else if(strcmp("cd", program) == 0) handleCD(command);
          else if(strcmp("path", program) == 0) path = handlePath(path, command);
          else {
            executeCommand(command, path);
          }
          free(program);
        }
        freeDoubleCharArray(commands, numCommands);
        free(commands);
        free(input);
      }

      break;
    case 2: ;
      // read from argv[1]
      FILE * fp = fopen(argv[1], "r");
      if(fp == NULL) {
        errMessage();
        exit(1);
      }
      char * input;
      size_t length = 0;
      ssize_t read;
      while((read = getline(&input, &length, fp)) != -1) {  
        char tempInput[length];
        strcpy(tempInput, input);
        // printf("%s", input);
        int numCommands = 0;
        char ** commands = getCommands(tempInput, &numCommands);

        for(int i = 0; i < numCommands; ++i) {
          char command[strlen(commands[i]) + 1];
          strcpy(command, commands[i]);
          if(strcmp("\n", command) == 0) break;
          char * program = getProgName(command);
          if(strcmp("exit", program) == 0) {
            freeDoubleCharArray(commands, numCommands);
            free(commands);
            free(program);
            free(input);
            free(path);
            handleExit(command);
          } else if(strcmp("cd", program) == 0) handleCD(command);
          else if(strcmp("path", program) == 0) path = handlePath(path, command);
          else {
            executeCommand(command, path);
          }
          free(program);
        }
        freeDoubleCharArray(commands, numCommands);
        free(commands);
        // free(input);
      }
      
      break;

    default:
      #ifdef DEBUG
        printf("Did not start up wish correctly. Entered too many arguments.\n"); 
      #endif
      errMessage();
    
  }
  return 0;
}
