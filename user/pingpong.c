#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    int p1[2];
    int p2[2];

    if(pipe(p1) == -1 || pipe(p2) == -1){
        printf("error creating pipes");
        exit(1);
    }

    if(fork() == 0){
        close(p1[1]);
        close(p2[0]);
        char byte;
        read(p1[0], &byte , 1);
        printf("%d: received ping\n", getpid());
        write(p2[1], &byte, 1);
        close(p1[0]);
        close(p2[1]);    
    }else{
        close(p1[0]);
        close(p2[1]);
        write(p1[1], "A", 1);
        char byte;
        read(p2[0], &byte, 1);
        printf("%d: received pong\n", getpid());
        wait(0);
        close(p1[1]);
        close(p2[0]);
    }

    exit(0);
    
}