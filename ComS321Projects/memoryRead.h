
#include <stdio.h>
#include <stdlib.h>
#include <memoryread.c>
#include <Opocodes.h>
#include <opocodes.c>

void readFile(int argc, char const *argv[]) {
    if(argc >= 2) {
        int i =1;
        while(i<argc)
            printf("%s\t \n", argv[i]);
            i=i+1;
        }

        FILE *file = fopen(argv[1], "f");

        if(file == 0) {
          printf("file not found ") 
        } else {

            int i=0;

            while ((i = fgetc( file ) ) != EOF ) {
              printf("%c", i );
            }
        }

        fclose(file);

    }

}