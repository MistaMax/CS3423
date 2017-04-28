#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSZ 1024

void main(int argc, char *argv[]){
    if (argc < 2){
        fprintf(stderr, "Usage: prog2 <file1> [file2] [file3] ... [file_n]");
        exit(-1);
    }

    int bytesread;
    int fd;
    char buf[BUFSZ];
    int i;
    for(i = 1; i < argc; i++){
        if((fd = open(argv[i], O_RDONLY)) == -1){
            perror("Failed to open file");
            exit(-2);
        }

        while((bytesread = read(fd, buf, BUFSZ)) > 0){
            write(STDOUT_FILENO,buf,bytesread);
        }
        
        if(bytesread < 0){
            perror("File error");
            exit(-3);
            close(fd);
        }
        close(fd);
    }
}
