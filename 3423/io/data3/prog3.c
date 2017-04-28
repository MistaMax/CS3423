#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#define BUFSZ 1024
void main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "Usage: prog3 <file-list>\n";
        exit(-1);
    }
    char buf[BUFSZ];
    int i, n;
    for(i = 1; i < argc; i++){
        if((fd = open(argv[i], O_RDONLY)) == -1){
            perror(argv[i]);
            exit(-1);
        }
        while((n = read(fd, buf, BUFSZ) > 0){

        }
    }
}
