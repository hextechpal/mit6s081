#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv){
    if(argc < 2){
        printf("please supply a command to execute");
    }

    char buf[512];
    while ((int b = read(1, &buf, sizeof(buf))) > 0){
        int child = fork();
        if (child == 0){
            char* eargs[argc + 1];
            int i;
            for (i = 0; i < argc; i++){
                strcpy(eargs[i], argv[i]);
            }
            strcpy(eargs[i], buf);
            memset(buf, 0, sizeof(buf));
            exec(eargs[0], eargs);
        }else if(child > 0){
            wait(0);
        }else{
            printf("failed to fork");
            continue;
        }
        
    }
    exit(0);
}