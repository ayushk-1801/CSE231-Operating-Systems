#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("Invalid input!");
        return 1;
    }

    FILE *src = fopen(argv[1], "r");
    if(src == NULL){
        perror("Unable to Open file");
        return 1;
    }
    int count = 0, word = 0;
    char c;
    while ((c = fgetc(src)) != EOF){
        if(!isspace(c)){
            word=1;
        }
        else{
            if(word){
                word = 0;
                count++;
            }
        }
    }
    if(word){
        count++;
    }
    fclose(src);

    printf("Total Word Count: %d\n", count);
    return 0;
}
