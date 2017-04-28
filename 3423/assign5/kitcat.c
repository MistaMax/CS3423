#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSZ 1024

void main(int argc, char *argv[]){
    if(argc > 4 || argc < 3){
        fprintf(stderr, "Usage: kitkat <file1> <file2> [file3]\n");
        exit(-1);
    }
    
    int fd1, fd2, fdout, bytesread, i;
    char buf[BUFSZ];

    if(argc == 4){
        close(STDOUT_FILENO);
        if((fdout = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644)) == -1){
            perror(argv[3]);
            exit(-2);
        }
    }
    if(*argv[1] == '-'){
        fd1 = STDIN_FILENO;
    }else if((fd1 = open(argv[1], O_RDONLY)) == -1){
        perror(argv[1]);
        exit(-3);
    }
    
    if(*argv[2] == '-'){
        if(*argv[1] == '-'){
            fprintf(stderr, "Error: Only one file may be read from stdin\n");
            exit(-4);
        }
        fd2 = STDIN_FILENO;
    }else if((fd2 = open(argv[2], O_RDONLY)) == -1){
        perror(argv[2]);
        close(STDOUT_FILENO);
        exit(-5);
    }

    while((bytesread = read(fd1, buf, BUFSZ)) > 0){
        write(STDOUT_FILENO, buf, bytesread);
    }

    if(bytesread < 0){
        perror(argv[1]);
        close(STDOUT_FILENO);
        close(fd1);
        exit(-6);
    }
    
    while((bytesread = read(fd2, buf, BUFSZ)) > 0){
        write(STDOUT_FILENO, buf, bytesread);
    }
    if(bytesread < 0){
        perror(argv[2]);
        close(STDOUT_FILENO);
        close(fd1);
        close(fd2);
        exit(-7);
    }

    close(fd1);
    close(fd2);
    close(STDOUT_FILENO);
}
