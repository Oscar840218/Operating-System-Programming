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
