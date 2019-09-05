/********** login.c file *************/
#include "ucode.c"

char buf[1024];

char *elements[32];

void split(char *line)
{
  char *cp;
  cp = line;
  argc = 0;
  int i;
  
  while (*cp != 0){
       while (*cp == ':') *cp++ = 0;        
       if (*cp != 0)
           elements[argc++] = cp;         
       while (*cp != ':' && *cp != 0) cp++;                  
       if (*cp != 0)   
           *cp = 0;                   
       else 
            break; 
       cp++;
  }
  elements[argc] = 0;

}

int main(int argc, char *argv[ ])
{
  int i, cur, pos=0;
  int fd, n=0;
  int pid = getpid();
  int in, out, err;
  char name[128], password[128], line[128];
  int last=0, time=0;
  int correct=0;

  close(0);
  close(1);

  in = open(argv[1], 0);
  out = open(argv[1], 1);
  err = open(argv[1], 2);

  settty(argv[1]);

  fd = open("/etc/passwd", O_RDONLY);

  read(fd, buf, 1024);
  
  while (1) {
    printf("login:");
    gets(name);
    printf("password:");
    gets(password);

    for (i=0; i<1024; i++) {
      if (buf[i] == 0) {
        break;
      }
      if (buf[i] == '\n') {
        for (cur=pos; cur<=i; cur++) {
          line[last] = buf[cur];
          last++;
        }
        line[last] = 0;

        //compare user, passwd
        
        split(line);
        /*printf("username=%s\n", elements[0]);
        printf("password=%s\n", elements[1]);*/
        if (strcmp(name, elements[0])==0 && strcmp(password, elements[1])==0) {
          printf("Login success!\n");
          correct = 1;
          break;
        }        


        last=0;
        pos = i+1;
        time++;
      }

    }

    pos = 0;

    if (correct) {
      chuid(atoi(elements[3]), atoi(elements[2]));
      chdir(elements[5]);
      close(fd);
      printf("Welcome to the system!");
      exec("sh in out err");
    }
    
  }

  printf("exit\n");
}
