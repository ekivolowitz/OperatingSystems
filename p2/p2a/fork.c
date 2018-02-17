#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int main(int argc, char * argv[]) {

    char * arg1[] = {"/bin/ls", "-l", NULL};
    char * arg2[] = {"/bin/cat", NULL};
    char * arg3[] = {"/bin/ls", "/home/e/ekivolowitz", NULL};
    char ** args[3];
    args[0] = arg1;
    args[1] = arg2;
    args[2] = arg3;

    for(int i = 0; i < 3; i++) {
        int rc = fork();
        if(rc < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } else if(rc == 0) {
            printf("Beginning child %d\n", i);
            execv(args[i][0], args[i]);
        } else {
            // stuff
            int wc = wait(NULL);
            printf("Hello, I am parent %d\n", wc);
        }
    }        


    
    return 0;
}
