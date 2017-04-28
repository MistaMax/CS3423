#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

struct cmd
{
    int redirect_in;        /* Any stdin redirection?       */
    int redirect_out;       /* Any stdout redirection?      */
    int redirect_append;    /* Append stdout redirection?   */
    int background;         /* Put process in background?   */
    int piping;             /* Pipe prog1 into prog2?       */
    char *infile;           /* Name of stdin redirect file  */
    char *outfile;          /* Name of stdout redirect file */
    char *argv1[10];        /* First program to execute     */
    char *argv2[10];        /* Second program in pipe       */
};

int cmdscan(char *cmdbuf, struct cmd *com);

void main(void){
    char buf[1024];
    struct cmd command;
    int fd[2];
    int fdin, fdout;

    while((gets(buf) != NULL)){
        if (cmdscan(buf,&command)){
            printf("Illegal Format: \n");
            continue;
        }
        switch(fork()){
        case -1:
            perror("Fork error");
            exit(-1);
        case 0:
            if(command.piping){
                pipe(fd);

                switch(fork()){
                case -1:
                    perror("Fork error");
                    exit(-1);
                case 0:
                    
                    close(fd[1]);
                    dup2(fd[0], STDIN_FILENO);
                    close(fd[0]);
                    
                    if(command.redirect_out){
                        if(command.redirect_append){
                            fdout = open(command.outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
                        } else {
                            fdout = open(command.outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        }
                        dup2(fdout, STDOUT_FILENO);
                        close(fdout);
                    }
                    execvp(command.argv2[0], command.argv2);
                    perror("Exec error");
                    exit(-1);
                    
                default:
                    close(fd[0]);
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[1]);

                    if(command.redirect_in){
                        fdin = open(command.infile, O_RDONLY);
                        dup2(fdin, STDIN_FILENO);
                        close(fdin);
                    }

                    execvp(command.argv1[0], command.argv1);
                    perror("Exec error");
                    exit(-1);
                }
            } else { // no piping
                if (command.redirect_in){
                    fdin = open(command.infile, O_RDONLY);
                    dup2(fdin, STDIN_FILENO);
                    close(fdin);
                }
                if (command.redirect_out){
                    if (command.redirect_append){
                        fdout = open(command.outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
                    } else {
                        fdout = open(command.outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
                    }
                    dup2(fdout, STDOUT_FILENO);
                    close(fdout);
                }
                
                execvp(command.argv1[0], command.argv1);
                perror("Exec error");
                exit(-1);
            }
        }
        if(command.background == 0) 
            wait(NULL);    
    
    }
    
}
