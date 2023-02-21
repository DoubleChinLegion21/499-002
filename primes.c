// Caleb Rivera
// criver7@gmu.edu
// G01114348

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
do_pipes(int p[])
{
  //create a new pipe for the child proceses
  int c_pipe[2];
  pipe(c_pipe);

  int primes[50];
  int cnt = 0;
  char buf[1];
  
  while ((read(p[0], buf, sizeof(buf))) != 0) {
    primes[cnt++] = buf[0];
  }
  read(p[0], buf, sizeof(buf));
  close(p[0]);
  
  if (cnt == 0) return; 
  int first = primes[0]; 
  printf("prime %d\n", first);
  
  for (int i = 1; i < cnt; i++) { 
    if (primes[i] % first != 0) {
      char p = primes[i];
      write(c_pipe[1], &p, 1);
    }
  }
  close(c_pipe[1]);
  
  int pid = fork();
  if (pid == 0) {
    //recurssion ultimately creating another child
    do_pipes(c_pipe);
  }  
}  
//The first process feeds the numbers 2-35 into the pipline. Each prime number is then found and arranged and sent into the next process.
int
main()
{
  //create a pipe
  int p[2];
  pipe(p);
  
  //loop through the different values, ultimately feeding them all into the second processs with "write"
  for (int i = 2; i <= 35; i++) {
    int n = i;
    write(p[1], &n, 1);
  }

  //p[1] is the write end, closing it since we are done writing
  close(p[1]);
  
  //call the second function
  do_pipes(p);
  
  //wait null esnures all children have exited before continuing the parent
  wait(0);
  exit(0);
}
