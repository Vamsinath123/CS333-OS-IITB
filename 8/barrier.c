/*----------xv6 sync lab----------*/
#include "types.h"
#include "x86.h"
#include "spinlock.h"
#include "defs.h"
#include "barrier.h"

//define any variables needed here
uint ba=1;
struct spinlock spin;
char* dummy = "kernel spinlock";

int
barrier_init(int n)
{
	initlock(&spin, dummy);
	ba=n;
  //to be done
  return 0;
}

int
barrier_check(void)
{
	acquire(&spin);
	ba=ba-1;
	if(ba!=0){
		sleep((void *)&ba, &spin);
	}
	else{
		wakeup((void *)&ba);
	}
	release(&spin);
  //to be done
  return 0;
}

/*----------xv6 sync lock end----------*/
