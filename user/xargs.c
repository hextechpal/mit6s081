#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("please supply a command to execute");
	exit(0);
    }
	
    char buf[512];
    int b;
    while ((b=read(0, &buf, sizeof(buf))) > 1){
	//printf("read=%s, b=%d\n", buf, b);
        int child = fork();
        if (child == 0){
            char* eargs[argc + 1];
            int i;
	    //printf("starting to copy args\n");
            for (i = 1; i < argc; i++){
                eargs[i-1] = argv[i];
            }
	    //printf("original args copied\n");
            eargs[argc-1] = buf;
	    eargs[argc] = 0;
	    //printf("args array created correctly\n");
            //memset(buf, 0, sizeof(buf));
	    //printf("mem reset successful\n");
	    for (int j = 0; j < argc+1; j++){
        	printf("j=%d %s\n",j, eargs[j]);
    	    }
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
