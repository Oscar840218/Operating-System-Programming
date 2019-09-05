extern MINODE minode[NMINODE];
extern MINODE *root;
extern PROC   proc[NPROC], *running;
extern char gpath[256];
extern char *name[64];
extern int n;
extern int fd, dev;
extern char line[256], cmd[32], pathname[256];
extern char all_path[128];
int ls_file(u16 ino); 


int ls(char* pathname) {
  int n = 0;
  char *cp;
  //printf("call ls\n");

  if (pathname[0]!=0) {
    tokenize(pathname);
    while (name[n]!=0) {
      n++;
    }
    ls_dir(name[n]);
  } else {
    ls_dir(running->cwd);
  }
  //search(root,"file1");
  //u16 ino = getino("/");
  //printf("ino = %d", ino);
  
    
}

int ls_dir(MINODE* mip) {
  u16 ino;
  int i;
  //MINODE *mip;
  INODE* ip;
  char buf[BLKSIZE];
  char c, *cp;
  DIR *dp;
  //ino = getino(dirname);
  //mip = iget(dev, ino);
  ip = (INODE*)mip->INODE;
  get_block(dev, ip->i_block[0], buf);
  cp = buf;
  dp = (DIR *)buf;

  for (i=0; i<12; i++){
       if ( ip->i_block[i] ){
         printf("i_block[%d] = %d\n", i, ip->i_block[i]);

         get_block(dev, ip->i_block[i], buf);
          cp = buf;
          dp = (DIR *)buf;

          while (cp < buf + BLKSIZE){
              c = dp->name[dp->name_len];  // save last byte
              dp->name[dp->name_len] = 0;
              printf("%s ", dp->name);
              
              ls_file(dp->inode);
              
              dp->name[dp->name_len] = c; // restore that last byte
              cp += dp->rec_len;
              dp = (DIR *)cp;
        }
     }
   }


}

int ls_file(u16 ino) {
  MINODE* mip = iget(dev, ino);
  INODE* ip = (INODE*)mip->INODE;
  u16 mode;
  int mask, k;
  mode = ip->i_mode;
 
  //printf("mode = %d\n", mode);
  if ((mode & 0xF000) == 0x4000)
      printf("d");

  if ((mode & 0xF000) == 0xA000)
      printf("l");
  else if ((mode & 0xF000) == 0x8000)
      printf("-");

  mask = 000400;
  for (k=0; k<3; k++){
    if (mode & mask)
       printf("r");
    else
       printf("-");
    mask = mask >> 1;
    if (mode & mask)
       printf("w");
    else
       printf("-");
    mask = mask >> 1;
    if (mode & mask)
       printf("x");
    else
       printf("-");
    mask = mask >> 1;
  }
  printf(" %d ", ip->i_links_count);
  printf(" %d  %d", ip->i_uid, ip->i_gid);
  printf("%d ", ip->i_size);
  printf("%d ", ctime(&ip->i_ctime));
  printf("%s", name);
}

int chdir(char* pathname) {
  u16 ino;
  MINODE *mip;

  if (pathname[0]==0) {
    running->cwd = root;
    strcpy(all_path, "/");
  } else if (strcmp(pathname, ".")==0) {
    printf("In current directory\n");
  } else if (strcmp(pathname, "..")==0) {
    if (running->cwd->ino == 2) {
      printf("It's root!\n");
      return 0;
    }
    u16 pino = findino(running->cwd, running->cwd->ino);
    printf("pino=%d", pino);
    MINODE* pip = iget(dev, pino);
    running->cwd = pip;
  } else {
    ino = getino(pathname);
    mip = iget(dev, ino);
    INODE* ip = (INODE*)mip->INODE;
    u16 mode;
    mode = ip->i_mode;
    printf("mode = %d\n", mode);
    if ((mode & 0xF000) == 0x4000) { //is directory
      //iput(running->cwd);
      running->cwd = mip;
    } else {
      printf("It is not a directory\n");
    }
  }
}

int pwd(MINODE* minode) {
  if (minode == root) {
    printf("/\n");
  } else {
    rpwd(minode);
  }
}

int rpwd(MINODE* mip) {
  u16 pino;
  MINODE* pip;
  char* myname;

  if (mip == root) {
    return;
  }

  pino = findino(mip, mip->ino);

  printf("pino = %d\n", pino);

  pip = iget(dev, pino);
 
  myname = findmyname(pip, mip->ino, myname);
  
  rpwd(pip);  

  printf("/%s", myname);
  printf("\n");  
}


