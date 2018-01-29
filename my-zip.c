#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DEBUG

void compress(FILE * fp) {
  #ifdef DEBUG
    printf("DEBUG> Entered compress\n");
  #endif

  char character;
  char newCharacter;
  int count = 0;  
  while((character = fgetc(fp)) != EOF) {
    while((newCharacter = fgetc(fp)) != EOF) {
      count++;
      // Handle case that new character is the EOF
      if(newCharacter == EOF) {
        #ifdef DEBUG
          printf("DEBUG> End of file. Count is %d, char is %c", count, character);
        #endif
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&character, sizeof(char), 1, stdout);
        break;  
      }
      // Handle the case that the new character equals the old character
      else if(newCharacter == character) {
        // ungetc(newCharacter, fp);
        continue;
      }
      // Handle the case that the new character does not equal the old character.
      else {
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&character, sizeof(int), 1, stdout);
        count = 0;
        ungetc(newCharacter, fp);
        break;
      }
    }
  }
}

int main(int argc, char * argv[]) {
  if(argc == 1) {
    printf("my-zip: file1 [file2 ...]\n");
    exit(1);
  }
  else if(argc == 2) {

    #ifdef DEBUG
      printf("DEBUG> Two arguments entered.\n");
    #endif
    FILE * fp = fopen(argv[1], "r");
    if(fp == NULL) {
      printf("cannot open file\n");
      exit(1);
    }
    #ifdef DEBUG
      printf("DEBUG> Calling compress()\n");
    #endif
    compress(fp);
    fclose(fp);
  }
  else {

    #ifdef DEBUG
      printf("DEBUG> More than one argument\n");
    #endif

    FILE * temp = fopen("temp.a", "w");

    if(temp == NULL) {
      #ifdef DEBUG
        printf("DEBUG> Couldn't open temp.a");
      #endif
    }
    #ifdef DEBUG
      printf("DEBUG> argc is %d\n", argc);
    #endif


    for(int i = 1; i < argc; i++) {
      FILE * fp = fopen(argv[i], "r");

      if(fp == NULL) {
        printf("Could not open %s\n", argv[i]);
      }

      #ifdef DEBUG
        printf("DEBUG> Opened file %s\n", argv[i]);
      #endif

      char * buffer;
      size_t len = 0;
      ssize_t read;

      while((read = getline(&buffer, &len, fp)) != -1) {
        #ifdef DEBUG
          printf("DEBUG> Writing %s to temp.a\n", buffer);
        #endif
        fputs(buffer, temp);
      }
      
      free(buffer);
      fclose(fp);
    }
    
    fclose(temp);
    temp = fopen("temp.a", "r");
    if(temp == NULL)
      #ifdef DEBUG
        printf("TEMP IS NULL\n");
      #endif
      printf("Cannot open file\n");
    compress(temp);
    fclose(temp);

  }

  return 0;
}
