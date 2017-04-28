#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define BUFFSIZE 1024

void main(int argc, char *argv[]){
    if(argc != 2){
        printf("Invalid argument: prog1 file\n");
        exit(-1);
    }

    int fd, bytesread, byteswritten;
    char buf[BUFFSIZE];
    char *tmpbuf;
    fd = open(argv[1], O_RDONLY);
    if(fd == -1){
        perror("Failed to open file");
        exit(-2);
    }
    
    while(1){
        bytesread = read(fd,buf,BUFFSIZE);
        if(bytesread == 0){
            break;
        }else if(bytesread == -1){
            perror("Read error");
            close(fd);
            exit(-3);
        }

        tmpbuf = buf;
        while(bytesread > 0){
            
            byteswritten = write(STDOUT_FILENO, tmpbuf, bytesread);

            if(byteswritten == -1){
                perror("Write error");
                close(fd);
                exit(-4);
            }

            bytesread -= byteswritten;
            tmpbuf +=byteswritten;
        }
    }

    close(fd);
}
