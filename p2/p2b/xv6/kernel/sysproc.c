#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"
#include "pstat.h"

extern int GLOB_TICKETS;
int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_settickets(void) {
  int number;
  argint(0, &number);
  if(number < 1) return -1;
//  cprintf("PID: %d | numTickets: %d | GLOB_TICKETS: %d\n", proc->pid, proc->numTickets, GLOB_TICKETS);
  GLOB_TICKETS += number;
  proc->numTickets = number;  
  // cprintf("PID: %d | numTickets: %d\n", proc->pid, proc->numTickets);
//  cprintf("PID: %d | numTickets: %d | GLOB_TICKETS: %d\n", proc->pid, proc->numTickets, GLOB_TICKETS);
  return 0;
}

int
sys_getpinfo(void) {
  int i = sizeof(struct pstat);
  char * pp;
  // int inuse[NPROC];   // whether this slot of the process table is in use (1 or 0)
  // int tickets[NPROC]; // the number of tickets this process has
  // int pid[NPROC];     // the PID of each process 
  // int ticks[NPROC];   // the number of ticks each process has accumulated 
  if(argptr(0, &pp, i) < 0) return -1;
  struct pstat * process;
  struct proc *p = NULL;
  process = (struct pstat *) pp;
  if(pp == NULL || pp == 0) return -1;
  int index = 0;
  
  PTABLE * table = getPTable();  
  
  for(p = table->proc; p < &table->proc[NPROC]; p++) {
    if(p->state != 0) process->inuse[index] = 1;
    else process->inuse[index] = 0;
    process->tickets[index] = p->numTickets;
    process->pid[index] = p->pid;
    process->ticks[index] = p->ticks;
    index += 1;
  }
  
  return 0;
}
