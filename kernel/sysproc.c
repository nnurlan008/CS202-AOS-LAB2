#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


// additions for lab1:
uint64 
sys_sysinfo(void) 
{ // sysinfo syscall definition
  int n;
  argint(0, &n);
  return sysinfo(n);
}
uint64 
sys_procinfo(void) 
{ // procinfo syscall definition
  struct pinfo *user_pinfo;
  argaddr(0, (void *)&user_pinfo);
  return procinfo(user_pinfo);
}


// additions for lab2:
uint64
sys_sched_statistics(void) {
  proc_sched_statistics();

  return 0; 
}

uint64
sys_sched_tickets(void) {
  int ticket;
  argint(0, &ticket);

  //if (!ticket) 
  //  return -1;

  int tickets = proc_set_sched_tickets(ticket);

  return tickets;
}
