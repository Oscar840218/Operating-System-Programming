/*******************************************************
*                      t.c file                        *
*******************************************************/
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define TRK 18
#define CYL 36
#define BLK 1024

#include "ext2.h"

typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;
GD    *gp;
INODE *ip;
DIR   *dp;

char buf1[BLK], buf2[BLK];
int color = 0x0A;
u8 ino;

main()
{ 
  u16    i, iblk, boot_node;
  char   c, temp[64];
  char *cp;

  prints("read block# 2 (GD)\n\r");
  getblk(2, buf1);

// 1. WRITE YOUR CODE to get iblk = bg_inode_table block number
  gp = (GD *)buf1;
  iblk = (u16)gp->bg_inode_table;
  prints("inode_block = "); putc(iblk+'0'); prints("\n\r"); 

// 2. WRITE YOUR CODE to get root inode
  prints("read inodes begin block to get root inode\n\r");
  getblk(iblk, buf1);
  ip = (INODE*)buf1+1;

//3. WRITE YOUR CODE to step through the data block of root inode
   prints("read data block of root DIR\n\r");
   getblk((u16)ip->i_block[0], buf1);
   dp = (DIR*) buf1;
   cp = buf1;
   while(cp < buf1 + 1024){
     deal_string(dp->name_len, dp->name);
     prints(dp->name);
     if (compare(dp->name, "boot")) {
        boot_node = dp->inode-1;
     }
     prints("\n\r");
     cp += dp->rec_len;
     dp = (DIR *)cp;
   }
// 4. print file names in the root directory /
   
}  

int prints(char *s)
{
    int index = 0;
    while (s[index]!=0) {
      putc(s[index]);
      index++;
    }
}

int deal_string(u16 l, char *s) {
    int index = (int)l;
    s[index] = '\0';
    
}

int compare(char *s, char *b) {
    int index = 0;
    while (s[index]!='\0') {
      if (s[index]!=b[index]) {
        return 0;
      } else {
        index++;
      }
    } 
    prints("<--This is boot");
    return 1;
}

int gets(char *s) 
{ 
}

int getblk(u16 blk, char *buf)
{
  // readfd( (2*blk)/CYL, ( (2*blk)%CYL)/TRK, ((2*blk)%CYL)%TRK, buf);
  readfd( blk/18, ((blk)%18)/9, ( ((blk)%18)%9)<<1, buf);
}
