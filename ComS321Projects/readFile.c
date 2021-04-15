#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int readFile(int argc, char const *argv[], int instructionArray[], FILE *file) {
    int c;
    int counter = 0;
    file = fopen(argv[1], "rb");
    
    if(file == 0) {
        
        printf("file could not be open");
        
    } else {
    
        //while not at the end of file
        while(!feof(file)) {
          fread(&c, sizeof(c), 1, file);
          c = be32toh(c);
          instructionArray[counter] = c;
          counter++;
          //printf("%02x\n", c);
        }
        
        //printf("--\n");

    };

    fclose(file);
    
    return counter;

}
