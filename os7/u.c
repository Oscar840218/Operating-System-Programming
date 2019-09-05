char buf1[BLKSIZE], buf2[BLKSIZE], buf3[BLKSIZE];

char path[64];
int nn;
char *name[32];  // at most 32 component names

int bmap, imap;
u16 iblk, ino;


int get_block(int fd, int blk, char buf[ ])
{
  lseek(fd, (long)blk*BLKSIZE, 0);
  read(fd, buf, BLKSIZE);
}


int tokenize(char *path)
{
  int i;
  char *cp;
  cp = path;
  nn = 0;

  while (*cp != 0){
       while (*cp == '/') *cp++ = 0;
       if (*cp != 0)
           name[nn++] = cp;
       while (*cp != '/' && *cp != 0) cp++;
       if (*cp != 0)
           *cp = 0;
       else
            break;
       cp++;
  }

  printf("n = %d : ", nn);
  for (i=0; i<nn; i++)
       printf("  %s  ", name[i]);
  printf("\n");
}

u16 search(INODE *ip) {
  int i;
  char c;
  DIR *dp;
  for (i=0; i<12; i++) {
    if (ip->i_block[i]) {
      get_block(fd, ip->i_block[i], buf3);
      dp = (DIR *)buf3;
      while ((char *)dp < &buf3[BLKSIZE]) {
        c = dp->name[dp->name_len]; //save last byte
        dp->name[dp->name_len] = 0; //make name into string
        printf("%s ",dp->name);
 	if (strcmp(dp->name, name) == 0) {
          printf("\n\r");
          return (dp->inode);
        }

        dp->name[dp->name_len] = c;
        dp = (char *)dp + dp->rec_len;
      }

    }
  }
}



int getino(int fd, char* pathname) {
  strcpy(path,pathname);
  tokenize(path);     // break up filename
  get_block(fd, 2 ,buf1);
  gp = (GD*)buf;
  iblk = gp->bg_inode_table;
  get_block(fd, iblk, buf2);
  ip = (INODE*)buf2+1;
  ino = search(ip);
  if (ino-1 < 0) {
    printf("Not found!\n");
    return 0;
  }
  
  return ino;
}

MINODE *iget(int fd, u16 ino) {
  
}

int iput(MINODE *mip) {

}
