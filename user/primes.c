#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


const int READ = 0;
const int WRITE = 1;

void generate(int left[2]){
    close(left[WRITE]);
    int prime; 
    if(read(left[READ], &prime, sizeof(int)) == 0){
        close(left[READ]);
        exit(0);
    }
    printf("prime %d\n", prime);
    int right[2];
    pipe(right);
    int child = fork();
    int tmp;
    if(child > 0){
        close(right[READ]);
        while(read(left[READ], &tmp, sizeof(int)) > 0){
            if(tmp % prime != 0){
                write(right[WRITE], &tmp, sizeof(int));
            }
        }
        close(right[WRITE]);
        close(left[READ]);
	    wait(0);
        exit(0);
    }else if(child == 0){
        generate(right);
        exit(0);    
    }else{
        close(left[READ]);
        close(right[READ]);
        close(right[WRITE]);
        exit(1);
    }
}



int main(int argc, char* argv[]){
    int p[2];
    if(pipe(p) == -1){
        printf("error initializing pipe");
        exit(1);
    }

    int child = fork();
    if(child == -1){
        close(p[READ]);
        close(p[WRITE]);
        exit(1);
    }else if(child > 0){
        close(p[READ]);
        for (int i = 2; i <= 35; i++){
            write(p[WRITE], &i, sizeof(int));
        }
        close(p[WRITE]);
        wait(0);
        exit(0);
    }else{
        generate(p);
        exit(0);
    }

}
