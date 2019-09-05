/********************************************************************
Copyright 2010-2017 K.C. Wang, <kwang@eecs.wsu.edu>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

/********************
#define  SSIZE 1024
#define  NPROC  9
#define  FREE   0
#define  READY  1
#define  SLEEP  2
#define  BLOCK  3
#define  ZOMBIE 4
#define  printf  kprintf
 
typedef struct proc{
  struct proc *next;
  int    *ksp;
  int    status;
  int    pid;

  int    priority;
  int    ppid;
  struct proc *parent;
  int    event;
  int    exitCode;
  int    kstack[SSIZE];
}PROC;
***************************/
#define NPROC 9
PROC proc[NPROC], *running, *freeList, *readyQueue;
int kprintf(char *fmt, ...);
int procsize = sizeof(PROC);
int body();

int init()
{
  int i, j; 
  PROC *p;
  kprintf("kernel_init()\n");
  for (i=0; i<NPROC; i++){
    p = &proc[i];
    p->pid = i;
    p->status = READY;
    p->next = p + 1;
  }
  proc[NPROC-1].next = 0; // circular proc list
  freeList = &proc[0];
  readyQueue = 0;

  printf("create P0 as initial running process\n");
  p = dequeue(&freeList);
  p->priority = 0;
  p->ppid = 0; p->parent = p;  // P0's parent is itself
  running = p;
  kprintf("running = %d\n", running->pid);
  printList("freeList", freeList);
}

int ksleep(int event) {
  int SR = int_off();
  running->event = event;
  running->status = SLEEP;
  kprintf("%d is sleeping on %x...\n", running->pid, running->event);
  tswitch();
  int_on(SR); 
}

int kawake(PROC *p) {
  int SR = int_off();
  p->status = READY;
  enqueue(&readyQueue, p);
  int_on(SR);
}

int kwait(int *status) {
  int i;
  PROC *cur, *out, *p, *run;
 
  if (!running->child) {
    return -1;
  }

  cur = running->child;

  if (cur->status == ZOMBIE) {//head is zombie
    out = cur;
    out->next = 0; 
    running->child = cur->sibling;
    enFreeList(&freeList, out);
    return out->pid;
  }

  while (cur->sibling) {
    if (cur->sibling->status == ZOMBIE) {
      out = cur->sibling;
      out->next = 0;
      cur->sibling = cur->sibling->sibling;
      enFreeList(&freeList, out);
      return out->pid;
    }
    cur = cur->sibling;
  }
  
  
  ksleep(running);
  return -2;
}

void kexit(int exitValue)
{
  if (running->pid!=1) {
    printf("proc %d kexit\n", running->pid);
    
    if (running->child) {
      PROC *cur = &proc[1];
      if (cur->status == SLEEP) {
        kawake(cur);
      }
      cur = cur->child;
      while (cur->sibling) {
        cur = cur->sibling;
      }
      cur->sibling = running->child;
      running->child = 0;
    }
    running->status = ZOMBIE;
    running->exitCode = exitValue;
    if (running->parent->status == SLEEP) {
      kawake(running->parent);
    }
    //enqueue(&freeList, running);   // putproc(running);
    tswitch();
  }
  
}
  
PROC *kfork(int func, int priority)
{
  int i;
  PROC *p = dequeue(&freeList);
  if (p==0){
    printf("no more PROC, kfork failed\n");
    return 0;
  }
  p->status = READY;
  p->priority = priority;
  p->ppid = running->pid;
  p->parent = running;
  // set kstack to resume to body
  // stack = r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r14
  //         1  2  3  4  5  6  7  8  9  10 11  12  13  14
  for (i=1; i<15; i++)
      p->kstack[SSIZE-i] = 0;
  p->kstack[SSIZE-14] = p->pid;
  p->kstack[SSIZE-13] = p->ppid;
  p->kstack[SSIZE-12] = (int)func;
  p->kstack[SSIZE-11] = p->priority;
  p->kstack[SSIZE-1] = (int)func;  // in dec reg=address ORDER !!!
  p->ksp = &(p->kstack[SSIZE-14]);
  enqueue(&readyQueue, p);
  if (!running->child) {
    running->child = p;
  } else {
    PROC *cur = running->child;
    while (cur->sibling) {
      cur = cur->sibling;
    }
    cur->sibling = p;
  }
  printf("%d kforked a child %d : proc %d : ", running->pid, p->pid, running->pid);
  printChildrenList("child list", running->child);
  return p;
}

int scheduler()
{
  
  kprintf("proc %d in scheduler ", running->pid);
  if (running->status == READY)
      enqueue(&readyQueue, running);
  running = dequeue(&readyQueue);
  kprintf("next running = %d\n", running->pid);

}  


int body(int pid, int ppid, int func, int priority)
{
  
  char c; char line[64]; char t; char msg[128];
  int index = 0; int status;
  //int pid;
  
  while(1){
    pid = running->pid;
    if (pid==0) color=BLUE;
    if (pid==1) color=WHITE;
    if (pid==2) color=GREEN;
    if (pid==3) color=CYAN;
    if (pid==4) color=YELLOW;
    if (pid==5) color=WHITE;
    if (pid==6) color=GREEN;   
    if (pid==7) color=WHITE;
    if (pid==8) color=CYAN;

    kprintf("Info--> pid=%d : ppid=%d : func=%d : priority=%d\n", pid, ppid, func, priority);
    kprintf("proc %d resume to body()\n", running->pid);
    kprintf("proc %d running: parant = %d : ",running->pid, running->ppid);
    printChildrenList("child list", running->child);
    //kprintf("sibling = %d\n", running->sibling->pid);
    //printList("freeList", freeList);
    printList("readyQueue", readyQueue);
    kprintf("proc %d running, parent = %d", running->pid, running->ppid);
    kprintf("input a char [s|f|q|w] : ");
    c = kgetc(); 
    printf("%c\n", c);

    switch(c){
      case 's': tswitch();           break;
      case 'f': kfork((int)body, 1); break;
      case 'q':
        index = 0;
        kprintf("Enter an exit value: ");
        while (1) {
          t = kgetc();
          if (t=='\r' || index==129) {
            break;
          }
          printf("%c", t);
          msg[index++] = t;
        }
        kprintf("\n");
        kprintf("Integer value = %d\n", geti(msg));
        kexit(geti(msg));     
        break;
        case 'w':
          pid = kwait(&status);
          if (pid == -1) {
            kprintf("Error: %d has no child\n", running->pid);
          } else if (pid != -2) {
            kprintf("Delete ZOMBIE process %d...\n", pid);
          } 
          
          break;
    }
  }
}
