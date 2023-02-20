// Caleb Rivera
// criver7@gmu.edu
// G01114348

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
catdir(char *predix, char *name, char *buf)
{
  memcpy(buf, predix, strlen(predix));
  char *p = buf + strlen(predix);
  *p++ = '/';
  memcpy(p, name, strlen(name));
  p += strlen(name);
  *p++ = 0;
}


void
find(int fd, char *dir, char *name) {
  struct dirent de;
  
  while(read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      continue;
    struct stat st;
    char path[512];
    catdir(dir, de.name, path);
    //the section above compares 2 strings and their size and if they match then it continues.
    if(de.inum == 0)
        continue;
    if(stat(path, &st) < 0){
        printf("find: cannot stat %s\n", path);
        continue;
    }
    if (st.type == T_FILE && strcmp(de.name, name) == 0) {  //string type must match the file and its name, this is where the actualy compare happens
      printf("%s\n", path);
    } else if (st.type == T_DIR) {
      int subfd;
      if((subfd = open(path, 0)) < 0){
        printf("find: cannot open %s\n", path);
        continue;
      }  //if no match is found then "cannot open" is produced
      find(subfd, path, name);
    }

  }
}


int
main(int argc, char *argv[])
{
  if (argc != 3) {
    fprintf(2, "To use find, give two additional commands, location and string of where you want to find\n");
    exit(1);
  } 

  char dir[DIRSIZ + 1];
  char name[DIRSIZ + 1];

  if (strlen(argv[1]) > DIRSIZ || strlen(argv[2]) > DIRSIZ) {
    fprintf(2, "dir or name too long...\n");
    exit(1);
  }  //if the argument value is larger than the directory size then the directory is wrong.

  memcpy(dir, argv[1], strlen(argv[1])); 
  memcpy(name, argv[2], strlen(argv[2]));
  //directory length is copied to memory and the name length.
  
  int fd;
  struct stat st;

  if((fd = open(dir, 0)) < 0){
    fprintf(2, "cannot open directory %s\n", dir);
    exit(1);
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "cannot stat directory %s\n", dir);
    close(fd);
    exit(1);
  }

  if (st.type != T_DIR) {
    printf("%s is not a dir\n", dir);
  } else {
    find(fd, dir, name);
  }
  //If the string type is not tied to a directory then it is not recognized as a directory.
  exit(0);
}
