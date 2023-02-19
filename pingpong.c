//imports
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"	

//function initialization
int
main()
{
	
  int p[2];
  char buf[20];
  //pipes allow programs to communicate
  pipe(p);

  //fork creates a new child process
  int pid = fork();
  if (pid == 0) {
    //p[1] is the write end
    write(p[1], "ping", 4);
    printf("%d: received ping\n", getpid());
  } 
  else {
    wait(0);
    //p[0] is the read end
    read(p[0], buf, 4);
    printf("%d: received pong\n", getpid());
  }	
  exit(0);
}

