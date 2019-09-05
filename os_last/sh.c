/********** sh.c file *************/
#include "ucode.c"

char buf[1024];
char *elements[32];
char file1[32], file2[32];
int contain_direct;
char arrow[2];

void split(char *line)
{
  char *cp;
  cp = line;
  argc = 0;
  int i;
  
  while (*cp != 0){
       while (*cp == ' ') *cp++ = 0;        
       if (*cp != 0)
           elements[argc++] = cp;         
       while (*cp != ' ' && *cp != 0) cp++;                  
       if (*cp != 0)   
           *cp = 0;                   
       else 
            break; 
       cp++;
  }
  elements[argc] = 0;
  
}

int check_direct(char cmd[]) {
  int n=0;
  char tmp[128];
  strcpy(tmp, cmd);
  split(tmp);
  while (elements[n]!=0) {
    if (strcmp(">", elements[n])==0 || strcmp(">>", elements[n])==0) {
      strcpy(arrow, elements[n]);
      strcpy(file1, elements[n-1]);
      strcpy(file2, elements[n+1]);
      return 1;
    }
    n++;
  }
  return 0;
}


int do_cat_direct(char file[]) {
   int n = 0, in_fd;
   char words[128];
   while (1) {
     read(0, words, 1);
     mputc(words[0]);
     buf[n++] = words[0];
     if (words[0] == ' ') {
	buf[n] = '\0';
	in_fd = open(file, O_RDWR);
        if (in_fd < 0) {
	  creat(file);
	  in_fd = open(file, O_RDWR);
	}
	write(in_fd, buf, n);
	close(0);
        //dup2(in_fd, 0);
        close(in_fd);
	break;
     }
  }  
}

int do_double_arrow_direct() {
  /*int fd_in, fd_out;
  int n1, n2, i, j=0;
  char bufin[20480], bufout[20480];

  fd_in = open(file1, O_RDONLY);
  fd_out = open(file2, O_RDWR);
  if (fd_in < 0) {
    printf("can not find %s", file1);
    exit(0);
  }
  if (fd_out < 0) {
    printf("can not find %s", file2);
    exit(0);
  }
  n1 = read(fd_out, bufout, 20480);
  n2 = read(fd_in, bufin, 204800);
  for (i=n1; i<n1+n2; i++) {
    bufout[i] = bufin[j];
    j++;
  }
  write(fd_out, bufout, strlen(bufout));
  close(fd_in);
  close(fd_out);*/
}

int main(int argc, char *argv[ ])
{
  int i=0;
  int fd, n=0, status;
  int in_fd, out_fd;
  int contain_pipe = 0, pipe_loc=0;
  int contain_direct;
  int pd[2];
  int pid;
  int cur=0;
  char cmd[32], cmd1[32], cmd2[32];
  char words[128];

  while(1) {
    printf("\nLC's sh: ");
    gets(cmd);
 
    if (!strcmp(cmd, "logout")) {
       exit(0);
    }
    
    pid = fork();
    if (pid) {
      pid = wait(&status);
    } else {
      //analyse cmd 1.io direction 2.pipe
      
      while (cmd[i]!=0) {
        if (cmd[i]=='|') {
          contain_pipe = 1;
          pipe_loc = i;
          break;
        }
        i++;
      }
      
      if (contain_pipe) {
        for (i=0; i<pipe_loc-1; i++) {
          cmd1[cur] = cmd[i];
          cur++;
        }
        cmd1[cur] = '\0';
        cur = 0;
        for (i=pipe_loc+2; i<strlen(cmd); i++) {
          cmd2[cur] = cmd[i];
          cur++;
        }
        cmd2[cur] = '\0';


        pipe(pd);
        pid = fork();
        if (pid) {
 	  close(pd[1]);
	  dup2(pd[0], 0);
	  exec(cmd2);
	} else {
	  close(pd[0]);
	  dup2(pd[1], 1);
	  exec(cmd1);
	}

      } else {
        contain_direct = check_direct(cmd);
        printf("contain_direct = %d\n", contain_direct);
        if (contain_direct) {

	  if (strcmp(arrow, ">") == 0) {
	    if (strcmp(file1, "cat") == 0) {
              do_cat_direct(file2);
	    } else {
	      in_fd = open(file1, O_RDONLY);
              close(0);
              dup2(in_fd, 0);
              close(in_fd);

	      out_fd = open(file2, O_RDWR);
              close(1);
              dup2(out_fd, 1);
              close(out_fd);
	    }
	   } else if (strcmp(arrow, ">>") == 0) {
              printf("%s %s", file1, file2);
	      do_double_arrow_direct();
	   }
          
	}

        exec(cmd);
        
      }
      
    }

  }
}
