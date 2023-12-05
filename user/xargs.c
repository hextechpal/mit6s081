#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_ARG_LEN 512

int main(int argc, char* argv[]){
    
    if(argc < 2){
        printf("please supply a command to execute");
	    exit(0);
    }
	
    char arg[MAX_ARG_LEN];
    char buf;
    int buf_index = 0;

    char* args[argc+1];

    int b;

    for (int i = 1; i < argc; i++){
        args[i-1] = argv[i];
    }
    args[argc-1] = arg;
	args[argc] = 0;
    
    while ((b=read(0, &buf, 1)) > 0){
	    
        if(buf == '\n' || buf == ' '){
            arg[buf_index] = 0;

            int child = fork();
            if(child == 0){
                exec(args[0], args);
            }else if(child > 0){
                wait(0);
                buf_index = 0;
            }else{
                fprintf(2, "cannot fork");
            }    
        }else{
            arg[buf_index] = buf;
            buf_index++;
        }
    }
    exit(0);
}
