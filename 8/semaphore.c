/*----------xv6 sync lab----------*/
#include "types.h"
#include "x86.h"
#include "defs.h"
#include "semaphore.h"

char* dum = "kernel spinlock";

int
sem_init(int index, int val)
{
	initlock(&li[index],dum);
	acquire(&li[index]);
	sem[index]=val;
	release(&li[index]);
  //to be done
  return 0;
}

int
sem_up(int index)
{
	acquire(&li[index]);
	sem[index]=sem[index]+1;
	newwakeup((void *) (&sem[index]));
	release(&li[index]);
  //to be done
  return 0;
}

int
sem_down(int index)
{
	acquire(&li[index]);
	sem[index]=sem[index]-1;
	if(sem[index]<0){
		sleep((void *) (&sem[index]),&li[index]);
	}
	release(&li[index]);
  //to be done
  return 0;
}

/*----------xv6 sync lab end----------*/
