
// queue.c file
extern PROC *freeList;
// WRITE YOUR OWN functions:
int kprintf(char *fmt, ...);

int enqueue(PROC **queue, PROC *p)
{
  int SR = int_off();  // IRQ interrupts off, return CPSR
    // enter p into *queue by priority; PROCs with the same priority by FIFO;
  PROC *cur = *queue;
  if (*queue) {
      if (p->pid == 0) {
        (*queue)->next = p;
      } else if ((*queue)->pid == 0) {
        p->next = *queue;
        *queue = p;
      } else {
        if (cur->priority < p->priority) {
          p->next = cur;
          cur = p;
          return;
        }
        while (cur->next->priority >= p->priority) {
          cur = cur->next;
        }
        p->next = cur->next;
        cur->next = p;
      }
  } else {
    *queue = p;
  }

  int_on(SR);          //  restore CPSR 
}

PROC *dequeue(PROC **queue)
{
  int SR = int_off();  // IRQ interrupts off, return CPSR
    // remove the FISRT element from *queue;
  PROC *head = *queue;
  *queue = (*queue)->next;
  head->next = 0;

  int_on(SR);          //  restore CPSR 
  return head;
}

int printList(char *name, PROC *p)
{
   kprintf("%s = ", name);
   while(p){
     kprintf("[%d%d]->", p->pid, p->priority);
     p = p->next;
  }
  kprintf("NULL\n");
}

int printChildrenList(char *name, PROC *p)
{
   kprintf("%s = ", name);
   while(p){
     kprintf("[%d]->", p->pid);
     p = p->sibling;
  }
  kprintf("NULL\n");
}

int enFreeList(PROC **queue, PROC *p) {
  int SR = int_off();
  PROC *cur = *queue;
  while (cur->next) {
    cur = cur->next;
  }
  p->status = FREE;
  cur->next = p;
  int_on(SR);
}

