#include <stdio.h>
#include <stdlib.h>
#define NMAX 10

void main(int argc, char *argv[]){

    int i, j, n, parent, fd[10][2], rng;
    

    /* open pipes */
    for(i = 0; i < NMAX; i++){
        pipe(fd[i]);
    }
    

    /* fork loop */
    for(i = 0; i < NMAX-1; i++){
        if((parent = fork()) == -1){
            perror("forkerror");
            exit(-1);
        }    
        if(parent) break;
    }


    /* close unneeded read-ends on pipes */
    for(n = 0; n < NMAX; n++){
        if(n != i){
            close(fd[n][0]);
        }
    }


   /* write block */
    srand(5972261*i);
    rng = RAND_MAX/10;
    rng *= 10;
    for (n=0; n<12; n++){
        do {
            j = rand()%NMAX;
        }
        while(j >= rng || i ==j);
        write(fd[j][1], &i, sizeof(i));
    }


    /* close write-ends on pipes */
    for(j = 0; j < NMAX; j++)
        close(fd[j][1]);


    /* read block */
    while((n = read(fd[i][0], &j, sizeof(j))) > 0){
        printf("process%d has received a message from process%d\n", i, j);
    }
    if(n == -1)
        fprintf(stderr, "read error in process%d\n", i);
    

    /* exit */
    close(fd[i][0]);
    exit(0);
}
