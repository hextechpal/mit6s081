#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* path, char* name){
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  //printf("path=%s, name=%s\n", path, name);
  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    printf("first argument must be a directory");
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      //printf("path=%s, cmp=%d\n", fmtname(buf), strcmp(de.name, "."));
      if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0){
	      continue;
      }
      
      if(st.type == T_DIR){
	      find(buf, name);
      }else if(strcmp(de.name, name) == 0){
	      printf("%s/%s\n", path, de.name);
      }
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[]){

  if(argc != 3){
    printf("please supply correct args");
    exit(0);
  }
  find(argv[1], argv[2]);
  exit(0);
}
