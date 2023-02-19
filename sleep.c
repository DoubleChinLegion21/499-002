#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"	

int
main(int argc, char *argv[])
{

  if(argc < 2){
    fprintf(2, "Usage did not work, please enter a valid number as an argument\n");
    exit(-1);
  }
  
  int val = atoi(argv[1]);
  if(val == 0) {
    fprintf(2, "Please enter a number that is greater than zero\n");
    exit(-1);
  }

  sleep(val);
  exit(0);
}

