/********** more.c file *************/
#include "ucode.c"

char buf[20480];

int do_pipe_more(int fd, char buf[]) {
  int i=0, l=1;
  char c;
  char line[512], cmd[20];
  while (1) {
    buf[0] = 0;
    read(0, buf, 1);
    //mputc(buf[0]);
    if (buf[0]==0) {
      break;
    }
    line[i++] = buf[0];
    if (buf[0] == '\n') {
      line[i] = '\0';
      printf("%s", line);
      i=0;
      l++;
    }
    if (l == 23) {
      
      l = 0;
      while (1) {
        c = getc();
        if (c == '\n') {
          break;	
	} else if (c == ' ') {
	  break;	
	}
       
      }
    }
  }
}

int do_more(int fd, char buf[], int buf_size) 
{
  char c;
  int cur, pos=0, i=0, last=0, n, l=1;
  int flag1=0, flag2=0;
  char line[512];
  n = read(fd, buf, 20480);
  
  while (buf[i]!=0) {
    
    if (buf[i]=='\n' || buf[i+1]==0) {
      
      for (cur=pos; cur<=i; cur++) {
        line[last] = buf[cur];
        last++;
      }
      line[last] = '\0';
      
      printf("%s", line);
      pos = i+1;
      flag2 = 1;
      l++;
   }
   if (l==23 || (flag1==1 && flag2==1)) {
     l = 0;
     while (1) {
       c = getc();
       if (c == '\r') {
         flag1 = 1;
         break;
       } else if (c == ' ') {
         flag1 = 0;
	 break;
       }
     }
   }

   flag2 = 0;
   last = 0;
   i++;
  }
  
}


int main(int argc, char *argv[ ])
{
  int i;
  int fd, n;
  int pid = getpid();
  printf("*****************************Oscar's more program***************************\n", pid);
  
  if (argc == 1) {
    do_pipe_more(0, buf);
  } else {
      fd = open(argv[1], O_RDONLY);
      if (fd < 0)
        exit(1);
      do_more(fd, buf, 20480);
      close(fd);
  }
  
}
