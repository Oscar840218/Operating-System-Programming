/********** grep.c file *************/
#include "ucode.c"

char buf[20480];

int check_contain(char line[], int param) {
  int i, j, k=0;
  int slen=0, linelen=0;
  char select[64];
  slen = strlen(argv[param]);
  linelen = strlen(line);
  if (linelen > slen) {
    for (i=0; i<linelen-slen; i++) {
      for (j=i; j<i+slen; j++) {
        select[k] = line[j];
	k++;  
      }
      select[k] = '\0';
      if (strcmp(select, argv[param])==0) {
	 printf("%s", line);
	 break;
      }
      k=0;
    }
  } 
}

int do_pipe_grep(int fd, char buf[]) {

  int i=0;
  char line[512];
  
  while (1) {
    int slen=0, linelen=0;
    buf[0] = 0;
    read(fd, buf, 1);
    
    if (buf[0]==0) {
      break;
    }
    line[i++] = buf[0];
    if (buf[0]=='\n') {
      
      line[i] = '\0';
      check_contain(line, 1);
      i=0;
    }
  }

  
}

int do_grep(int fd, char buf[], int param) {

  int i=0;
  int cur, pos=0, last=0;
  char line[512];
  int slen, linelen;

  read(fd, buf, 20480);
  
  while (buf[i]!=0) {
      if (buf[i]=='\n' || buf[i+1]==0) {
        
        slen=0;
	linelen=0;
    
        for (cur=pos; cur<=i; cur++) {
          line[last] = buf[cur];
          last++;
        }
        line[last] = '\0';
        
        check_contain(line, param);

        pos = i+1;
    }
    last = 0;
    i++;
  }
}


int main(int argc, char *argv[ ])
{
  
  int fd, n;
  int pid = getpid();
  printf("*************Oscar's grep program*****************\n");
  
  if (argc == 2) {
    do_pipe_grep(0, buf);
   
  } else if (argc == 3) {
    fd = open(argv[1], O_RDONLY);
     if (fd < 0)
      exit(1);
    
    do_grep(fd, buf, 2);
    close(fd);
    
 } 


}
