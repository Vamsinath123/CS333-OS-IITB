/*----------xv6 sync lab----------*/
#include "types.h"
#include "x86.h"
#include "uspinlock.h"

//TODO: define an array of NLOCK uspinlocks

uint arr[NLOCK];

int
uspinlock_init(void)
{
  // Initialize all locks to unlocked state
  // To be done.
  for(int i=0;i<NLOCK;i++){
    xchg(&arr[i],0);
  }
  return 0;
}

int
uspinlock_acquire(int index)
{
  while(xchg((&arr[index]),1)!=0){
    ;
  }
  // To be done.
  return 0;
}

int
uspinlock_release(int index)
{
  arr[index]=0;
  // To be done.
  return 0;
}

int
uspinlock_holding(int index)
{
  // To be done.
  //Return 0 if lock is free, 1 if lock is held
  return arr[index];
  return 0;
}
/*----------xv6 sync lock end----------*/
