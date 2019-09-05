/*********************************************************************
        You MAY use the util.o file to get a quick start.
 BUT you must write YOUR own util.c file in the final project
 The following are helps of how to wrtie functions in the util.c file
*********************************************************************/


/************** util.c file ****************/


/**** globals defined in main.c file ****/

extern MINODE minode[NMINODE];
extern MINODE *root;
extern PROC   proc[NPROC], *running;
extern char gpath[256];
extern char *name[64];
extern int n;
extern int fd, dev;
extern int nblocks, ninodes, bmap, imap, inode_start;
extern char line[256], cmd[32], pathname[256];
char buf1[BLKSIZE], buf2[BLKSIZE], buf3[BLKSIZE];


int get_block(int dev, int blk, char *buf)
{
  lseek(dev, (long)blk*BLKSIZE, 0);
  read(dev, buf, BLKSIZE);

}   

int put_block(int dev, int blk, char *buf)
{
  lseek(dev, (long)blk*BLKSIZE, 0);
  write(dev, buf, BLKSIZE);

}   

int tokenize(char *pathname)
{
  int i;
  char *cp;
  strcpy(gpath, pathname);
  cp = gpath;
  n = 0;

  while (*cp != 0){
       while (*cp == '/') *cp++ = 0;
       if (*cp != 0)
           name[n++] = cp;
       while (*cp != '/' && *cp != 0) cp++;
       if (*cp != 0)
           *cp = 0;
       else
            break;
       cp++;
  }

  printf("n = %d : ", n);
  for (i=0; i<n; i++)
       printf("  %s  ", name[i]);
  printf("\n");
  
// tokenize pathname into n components: name[0] to name[n-1];
}

MINODE *iget(int dev, int ino)
{
  MINODE *mip, *tmip;
  INODE *ip;
  int i;
  u16 blk, offset;
  // return minode pointer to loaded INODE
  /*(1). Search minode[ ] for an existing entry with the needed (dev, ino):
       if found: inc its refCount by 1;
                 return pointer to this minode;*/
  
  for (i=0; i<NMINODE; i++) {
    tmip = &minode[i];
    if (tmip->ino == ino) {
      tmip->refCount++;
      //give tmip0>INODE content
      blk = (ino-1) / 8 + inode_start;
      offset = (ino-1) % 8;
      
      printf("iget: ino=%d blk=%d offset=%d\n", ino, blk, offset);

      get_block(dev, blk, buf3);

      ip = (INODE *)buf3 + offset;
      tmip->INODE = ip; 
      return tmip;
    }
  }
  /*(2). // needed entry not in memory:
       find a FREE minode (refCount = 0); Let mip-> to this minode;
       set its refCount = 1;
       set its dev, ino*/
  for (i=0; i<NMINODE; i++) {
    tmip = &minode[i];
    if (tmip->refCount == 0) {
      mip = tmip;
      mip->refCount = 1;
      mip->dev = dev;
      mip->ino = ino;
      break;
    }
  }
  
 // (3). load INODE of (dev, ino) into mip->INOiDE:

       // get INODE of ino a char buf[BLKSIZE]    
       blk    = (ino-1) / 8 + inode_start;
       offset = (ino-1) % 8;

       printf("iget: ino=%d blk=%d offset=%d\n", ino, blk, offset);

       get_block(dev, blk, buf3);

       ip = (INODE *)buf3 + offset;
       mip->INODE = ip;  // copy INODE to mp->INODE
      
       return mip;
}


int iput(MINODE *mip) // dispose a used minode by mip
{
 char* buf;
 mip->refCount--;
 
 if (mip->refCount > 0) return;
 if (!mip->dirty)       return;
 
 // YOUR C CODE: write INODE back to disk
// buf = (char)mip->INODE;
 u16 ino, blk;
 ino = mip->ino;
 blk = (ino-1) / 8 + inode_start;
 buf = (char *)mip->INODE;
 put_block(dev, blk, buf);
} 


// serach a DIRectory INODE for entry with given name
int search(MINODE *mip, char *name)
{
  int i;
   char c, *cp;
   DIR  *dp;
   INODE *ip = mip->INODE;
   for (i=0; i<12; i++){
       if ( ip->i_block[i] ){
         printf("i_block[%d] = %d\n", i, ip->i_block[i]);

         get_block(dev, ip->i_block[i], buf2);
          cp = buf2;
          dp = (DIR *)buf2;

          while (cp < buf2 + BLKSIZE){
              c = dp->name[dp->name_len];  // save last byte
              dp->name[dp->name_len] = 0;
              printf("%s ", dp->name);
              if ( strcmp(dp->name, name) == 0 ){
                 printf("FOUND %s\n", name);
                 return(dp->inode);
              }
              dp->name[dp->name_len] = c; // restore that last byte
              cp += dp->rec_len;
              dp = (DIR *)cp;
        }
     }
   }
   return 0;
}


// retrun inode number of pathname

int getino(char *pathname)
{ 
  u16 iblk, ino;
  int i, n;
   // SAME as LAB6 program: just return the inode's ino;
  strcpy(gpath,pathname);
  tokenize(gpath);  // break up filename

  n = 0;

  while (name[n]!=0) {
    n++;
  }

  get_block(dev, 2 ,buf1);
  gp = (GD*)buf1;
  iblk = gp->bg_inode_table;
  get_block(dev, iblk, buf2);
  ip = (INODE*)buf2+1;
  ino = 2;
  for (i=0; i<n; i++){
      ino = search(iget(dev, ino), name[i]);
      if (ino == 0){
          printf("can't find %s\n", name[i]);
          return(0);
      }
      ino--;
      get_block(dev, iblk+(ino / 8), buf2);      /* read block inode of me */
      ip = (INODE *)buf2 + (ino % 8);
   }
  return ino;

}


char* findmyname(MINODE *parent, u32 myino, char *myname) 
{
   // return name string of myino: SAME as search except by myino;
   // copy entry name (string) into myname[ ];
  char buf[BLKSIZE];
  int i;
  char c, *cp;
  DIR  *dp;

  INODE* ip = (INODE*)parent->INODE;
  for (i=0; i<12; i++){
       if ( ip->i_block[i] ){
         printf("i_block[%d] = %d\n", i, ip->i_block[i]);

         get_block(dev, ip->i_block[i], buf);
          cp = buf;
          dp = (DIR *)buf;
	  myino++;
          while (cp < buf + BLKSIZE){
              c = dp->name[dp->name_len];  // save last byte
              dp->name[dp->name_len] = 0;
              printf("%s ", dp->name);
              if (myino == dp->inode){
                 printf("FOUND %s\n", dp->name);
                 myname = dp->name;
		 return myname;
              }
              dp->name[dp->name_len] = c; // restore that last byte
              cp += dp->rec_len;
              dp = (DIR *)cp;
        }
     }
   }

}


int findino(MINODE *mip, u32 *myino) 
{
  // fill myino with ino of . 
  // retrun ino of ..
  u16 ino;
  myino = search(mip, ".");
  ino = search(mip, "..");
  return ino;
}

