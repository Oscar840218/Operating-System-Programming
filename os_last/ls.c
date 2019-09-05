/********** ls.c file *************/
#include "ucode.c"

char buf[1024];

int ls_file(char *filename) {
  STAT fstat, *sp = &fstat;
  u16 mode;
  int r, i, mask, k, fd;
  char buf[40960];
  char *s;

  if (r = stat(filename, sp) < 0) {
    exit(0);
  }
   
  mode = sp->st_mode;
  
  

  if ((mode & 0xF000) == 0x4000)
      mputc('d');

  if ((mode & 0xF000) == 0xA000)
      mputc('l');
  else if ((mode & 0xF000) == 0x8000)
      mputc('-');

  mask = 000400;
  for (k=0; k<3; k++){
    if (mode & mask)
       mputc('r');
    else
       mputc('-');
    mask = mask >> 1;
    if (mode & mask)
       mputc('w');
    else
       mputc('-');
    mask = mask >> 1;
    if (mode & mask)
       mputc('x');
    else
       mputc('-');
    mask = mask >> 1;
  }
  
  printf(" %d ", sp->st_nlink);
  printf("%d ", sp->st_uid);
  printf("%d ", sp->st_size);
  //printf("%d ", ctime(&sp->st_ctime));
  printf("%s ", filename);
  mputc('\n');
}

int ls_dir(char *dname) {
  char buf[1024];
  char name[256];
  int fd, n, i=0;
  DIR *dp;
  char *cp, c;
  fd = open(dname, O_RDONLY);
  if (fd < 0)
      exit(1);
  printf("\nfd %d of %s\n", fd, dname);
  read(fd, buf, 1024);
  dp = (DIR*) buf;
  cp = buf;
   while (cp < buf + 1024){
     c = dp->name[dp->name_len];  // save last byte
     dp->name[dp->name_len] = 0;
     ls_file(dp->name);
     dp->name[dp->name_len] = c; // restore that last byte
     cp += dp->rec_len;
     dp = (DIR *)cp;
   }
}

int main(int argc, char *argv[ ])
{
  int i;
  int fd, n;
  int pid = getpid();
  printf("*************Oscar's ls program***************");
  STAT mystat, *sp;
  int r;
  char *s;
  char filename[1024], cwd[1024];
  
  s = argv[1];
  if (argc == 1) {
    s = "./";
  }
  
  sp = &mystat;
  if (r = stat(s, sp) < 0) {
    exit(0);
  }
  strcpy(filename, s);
  /*if (s[0] != '/') {
    getcwd(cwd);
    strcpy(filename, cwd);
    strcat(filename, "/");
    strcat(filename, s);
  }*/
  
  if ((sp->st_mode & 0xF000) == 0x4000) {
    printf("This is dir\n");
    ls_dir(filename);
  } else {
    printf("This is file\n");
    ls_file(filename);
  }

  printf("exit\n");
}
