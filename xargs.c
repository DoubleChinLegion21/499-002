// Caleb Rivera
// criver7@gmu.edu
// G01114348

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

#define MAX_LEN_OF_ARG 512

void
copy_argv(char **ori_argv, int ori_argc, char *new_argv, char **argv)
{
  int k = 0;
  for (int i = 0; i < ori_argc; i++) {
    argv[k] = malloc(strlen(ori_argv[i]) + 1);
    //memory allocated to the string length of argv integer k
    memcpy(argv[k++], ori_argv[i], strlen(ori_argv[i]) + 1);
  }
  argv[k] = malloc(strlen(new_argv) + 1);
  //memory is allocated to the string length if the new argv
  memcpy(argv[k++], new_argv, strlen(new_argv) + 1);
}

int 
main(int argc, char *argv[])
{
  if (argc <= 1) {
    fprintf(2, "To use xargx command, do first_command [args1,2...] | xargs second_command [args1,2...]\n");
    exit(1);
  }

  char param[MAX_LEN_OF_ARG];
  int i = 0;
  char ch;
  int ignore = 0;
  while (read(0, &ch, 1) > 0) {
    if (ch == '\n') {
      if (ignore) {
        i = 0;
        ignore = 0;
        continue;
      }
      param[i] = 0;
      i = 0;

      int pid = fork();
      //If requirements are satsfied then the message in xv6 can be copied using echo and then xargs can be used to reply to the test file created.
      if (pid == 0) { //child process
        int cmd_argc = argc;
        
        char *cmd_argv[MAXARG];

        copy_argv(argv + 1, argc - 1, param, cmd_argv);
        cmd_argv[cmd_argc] = 0;
        
        exec(cmd_argv[0], cmd_argv);

        exit(0);
      } else {
        wait((int *)0);
      } 
    } else {
      //if the arguments length is in total longer than 512, than the arguments wont fit in the variables.
      if (!ignore && i >= MAX_LEN_OF_ARG - 1) {
        printf("xargs: too long arguments...\n");
        ignore = 1;
      }
      
      if (!ignore) {
        param[i++] = ch;
      }
    }
  }

  exit(0);
}
