/********** init.c file *************/
#include "ucode.c"

char buf[1024];

int console0, console1, console2, console;


int parent()
{
  int pid, status;
  while (1) {
    printf("INIT: wait for ZOMBIE child\n");
    pid = wait(&status);
    
    if (pid == console0) {
      printf("pid = %d console0!!!!\n", pid);
      console0 = fork();
      if (console0) {
        printf("pid in parent = %d!!!!\n", pid);
        continue; 
      } else {
        exec("login /dev/tty0");
      } 
    }
    if (pid == console1) {
      printf("pid = %d console0!!!!\n", pid);
      printf("INIT: fork a child console1\n");
      console1 = fork();
      if (console1) {
        continue; 
      } else {
        exec("login /dev/ttyS0");
      }
    }
    if (pid == console2) {
      printf("pid = %d console0!!!!\n", pid);
      printf("INIT: fork a child console2\n");
      console2 = fork();
      if (console2) {
        continue; 
      } else {
        exec("login /dev/ttyS1");
      }
    }
    
   printf("I just buried a orphan child proc %d", pid);
  }
} 

int main(int argc, char *argv[ ])
{
  int i, pid;
  int fd, n, in0, out0, in1, out1, in2, out2;
  pid = getpid();
 
  in0 = open("/dev/tty0", O_RDONLY);
  out0 = open("/dev/tty0", O_WRONLY);
  in1 = open("/dev/ttyS0", O_RDONLY);
  out1 = open("/dev/ttyS0", O_WRONLY);
  in2 = open("/dev/ttyS1", O_RDONLY);
  out2 = open("/dev/ttyS1", O_WRONLY);

  printf("fork a login proc on console\n");
  console0 = fork(); //2

  if (console0) {
    tswitch();
  } else {
    exec("login /dev/tty0");
  }

  console1 = fork(); //3

  if (console1) {
    tswitch();
  } else {
    exec("login /dev/ttyS0");
  }


  console2 = fork(); //4
  
  if (console2) {
    parent();
  } else {
    exec("login /dev/ttyS1");
  }

  printf("exit\n");
}
