#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
	rw->readers = 0;
	rw->writerpresent = false;
	rw->writers = 10;
  //	Write the code for initializing your read-write lock.
}

void ReaderLock(struct read_write_lock * rw)
{
	pthread_mutex_lock(&rw->rlock);
	while (rw->writerpresent){
		pthread_cond_wait(&rw->readlock,&rw->rlock);
	}
	rw->readers++;
	pthread_mutex_unlock(&rw->rlock);
  //	Write the code for aquiring read-write lock by the reader.
}

void ReaderUnlock(struct read_write_lock * rw)
{
	pthread_mutex_lock(&rw->rlock);
	rw->readers--;
	if (rw->readers == 0){
		pthread_cond_broadcast(&rw->writelock);
	}
	pthread_mutex_unlock(&rw->rlock);
  //	Write the code for releasing read-write lock by the reader.
}

void WriterLock(struct read_write_lock * rw)
{
	pthread_mutex_lock(&rw->rlock);
	while(rw->writerpresent || rw->readers>0)
		pthread_cond_wait(&rw->writelock,&rw->rlock);
	rw->writerpresent = true;
	pthread_mutex_unlock(&rw->rlock);
  //	Write the code for aquiring read-write lock by the writer.
}

void WriterUnlock(struct read_write_lock * rw)
{
	pthread_mutex_lock(&rw->rlock);
	pthread_cond_broadcast(&rw->writelock);
	pthread_cond_broadcast(&rw->readlock);
	rw->writerpresent = false;
	pthread_mutex_unlock(&rw->rlock);
  //	Write the code for releasing read-write lock by the writer.
}
