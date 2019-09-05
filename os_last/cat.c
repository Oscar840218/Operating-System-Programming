/********** cat.c file *************/
#include "ucode.c"

char buf[20480];

int cat_file(char buf[])
{
  int i=0, k=0;
  int pos=0, cur;
  int last=0;
  while (buf[i]!=0) {
      if (buf[i]=='\n' || buf[i+1]==0) {
        char line[512];
        for (cur=pos; cur<=i; cur++) {
          line[last] = buf[cur];
          last++;
        }
        line[last] = '\0';
	printf("%s", line);
        
        pos = i+1;
     }
     last = 0;
     i++;
  }
}

int main(int argc, char *argv[ ])
{
  int pid = getpid();
  int in, out, err;
  int n=0, fd;
  char words[128], wout[128];
  int j=0;

  printf("PROC %d running cat program\n", pid);
  printf("argc = %d\n", argc);
  printf("*********Oscar's cat program**************\n");
  

  if (argc == 1) {
   
    while (1) {
      read(0, words, 1);
      write(1, words, 1);
 
      wout[n++] = words[0];
     
      if (words[0]==' ') {
        mputc('\n');
	wout[n] = '\0';
       
	if (strcmp("exit", wout) == 0) {
  	  exit(0);
	}
	printf("%s", wout);
        mputc('\n');
        n = 0;
      }
    }
    
  } else {
    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
      exit(1);
  
    n = read(fd, buf, 20480);
    
    if (argc == 2) {
      cat_file(buf);
    } else {
      
      write(1, buf, n);
      
    }

    
    close(fd);
     
  }
  
  //close(fd);
  //printf("exit\n");
}
