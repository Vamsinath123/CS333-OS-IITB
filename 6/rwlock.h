#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>

using namespace std;

struct read_write_lock
{
	pthread_mutex_t rlock;
	pthread_cond_t writelock;
	pthread_cond_t readlock;
	int readers;
	bool writerpresent;
	int writers;
};

void InitalizeReadWriteLock(struct read_write_lock * rw);
void ReaderLock(struct read_write_lock * rw);
void ReaderUnlock(struct read_write_lock * rw);
void WriterLock(struct read_write_lock * rw);
void WriterUnlock(struct read_write_lock * rw);
