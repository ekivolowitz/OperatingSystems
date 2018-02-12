#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define DEBUG

void errMessage() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
  exit(1);
}



int main(int argc, char * argv[]) {
  
  switch(argc){
    case 1: ;
      // read from stdin
      while(1) {
        printf("wish> ");
        char * input = NULL;
        size_t length = 0;
        ssize_t read;
        read = getline(&input, &length, stdin);
        if(read == -1) printf("Error reading from stdin.\n");
        else {
          printf("%s", input);
          // look for redirection : use strchr to return a pointer to c in string s
            // find output file from there
            // find commands to run
          // look for parallel commands: if contains parallel, strtok_r to get sub commands. 
          // 
        }
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
