int mvrec(char *fdb, int num1, char *fdb2, int num2, int rsz){
    int i, fd1, fd2, numrec;
    fd1 = open(fdb, O_RDWR);
    fd2 = open(fdb2, O_RDWR);

    char buf[1024];

    numrec = lseek(fd2, 0, SEEK_END)/rsz;

    for(i = numrec -1; i >= num2; i--){
        lseek(fd2, i * rsz, SEEK_SET);
        read(fd2, buf, rsz);
        write(fd2, buf, rsz);
    }

    lseek(fd1, num1 * rsz, SEEK_SET);

    read(fd1, buf, rsz);
    lseek(fd2, num2 * rsz, SEEK_SET);
    if(write(fd2, buf, rsz) != rsz) return -1;

    numrec = lseek(fd1, 0, SEEK_END)/rsz;

    for(i = num1+1; i < numrec; i++){
        lseek(fd1, i * rsz, SEEK_SET);
        if(read(fd1, buf, rsz) != rsz) return -1;
        lseek(fd1, (i-1)*rsz, SEEK_SET);
        if(write(fd1, buf, rsz) != rsz) return -1;
    }

    ftruncate(fd1, (numrec-1) * rsz);
    close(fd1);
    close(fd2);
    return 0;
}






int extractrecs(char *fdb, int num1, int num2, char *fout, int rsz){
    int i, sz, fdout, fdin[2];
    char buf[1024];

    fdout = open(fout, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fdin[0] = open(fdb, O_RDONLY);
    fdin[1] = open(fdb, O_WRONLY);

    for(i = num1; i <= num2; i++){
        lseek(fdin[0], i * rsz, SEEK_SET);
        if(read(fdin[0], buf, rsz)!=rsz)return -1;
        if(write(fdout, buf, rsz) !=rsz)return -1;
        
        lseek(fdin[1], i * rsz, SEEK_SET);
        lseek(fdin[0], (i + num2 - num1) * rsz, SEEK_SET);

        if(read(fdin[0], buf, rsz)!=rsz) return -1;
        if(write(fdin[1], buf, rsz)!=rsz) return -1;
    }
    sz = lseek(fdin[0], 0, SEEK_END);
    ftruncate(fdin[1], sz - rsz*(num2-num1+1);
    close(fdin[0]);
    close(fdin[1]);
    close(fdout);
    return 0;
}






int pdopen(char *prog1, char *arg1, char *prog2, char *arg2){
    int fd[2];
    int fd2[2];
    pipe(fd);
    switch(fork()){
    case 0: 
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        pipe(fd2);
        break;
    case -1:
        perror("fork error");
        exit(-1);
    default:
        close(fd[0]);
        return fd[1];
    }

    switch(fork()){
    case 0:
        close(fd2[1]);
        dup2(fd2[0], STDIN_FILENO);
        close(fd2[0]);
        execlp(prog2, prog2, arg2, NULL);
        perror("exec error");
        break;
    case -1:
        perror("fork error");
        exit(-1);
    default:
        close(fd2[0]);
        dup2(fd2[1], STDOUT_FILENO);
        close(fd2[1]);
        execlp(prog1, prog1,  arg1, NULL);
        perror("exec error");
        exit(-1);
    }
}









int main(){
    int fd[2];
    int fdout;
    pipe(fd);

    switch(fork()){
    case 0:
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        fdout = open("/var/log/power.log", O_WRONLY | O_APPEND);
        dup2(fdout, STDOUT_FILENO);
        close(fdout);
        execlp("grep", "grep", "--line-buffered", "Bad", NULL);
        sys_err("exec error");
    case -1:
        sys_err("fork error");
    }

    close(fd[0]);
    dub2(fd[1], STDOUT_FILENO);
    close(fd[1]);
    execlp("journalctl", "journalctl", "-f", "--full", NULL);
    sys_err("exec error");
}







int main(){
    struct {
        int num;
        int pid;
        int index;
    } myMsg;
    int fd[33][2];
    int parent, n, ran;
    
    for(n = 0; n <= 32; n++){
        pipe(fd[n]);
    }

    for(n = 0; n < 32; n++){
        if((parent = fork()) == -1) sys_err("fork error");
        if(parent) break;
    }
    
    myMsg.index = n;
    for(n = 0; n < 33; n++){
        if(n == myMsg.index) {
            close(fd[n][1];
        }
        else{
            close(fd[n][0]);
        }
    }

    if(myMsg.index == 0){
        for(n = 1; n <=32; n++){
            ran = rand();
            write(fd[n][1], &ran, sizeof(int));
            close(fd[n][1]);
        }

        while(read(fd[0][0], &myMsg, sizeof(myMsg)) != 0){
            printf("Or %s %s %s", myMsg.num, myMsg.pid, myMsg.index);
        }
        close(fd[0][0]);
    }
    else{
        read(fd[myMsg.index][0], &ran, sizeof(int));
        close(fd[myMsg.index][0]);
        if(ran %2 == 0){
            myMsg.num = ran;
            myMsg.pid = getpid();
            write(fd[0][1], &myMsg, sizeof(myMsg));
        }
        for(n = 0; n < 33; n++){
            if(n != myMsg.index)
                close(fd[n][1]);
        }
    }


}










