#include "alloc.h"

#define NUMALLOC PAGESIZE/MINALLOC
int buff[NUMALLOC];
void* ptr;

int init_alloc(){
	for(int i=0;i<NUMALLOC;i++){
		buff[i]=-1;
	}
	ptr = mmap(0,PAGESIZE,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_ANON|MAP_PRIVATE,0,0);
	if(ptr == MAP_FAILED){
		return 43;
	}
	return 0;
}
int cleanup(){
	for(int i=0;i<NUMALLOC;i++){
		buff[i]=-1;
	}
	int y = munmap(ptr,PAGESIZE);
	if(y==-1){
		return 44;
	}
	return 0;	
}
char* alloc(int a){
	if(a%MINALLOC!=0){
		return NULL;
	}
	a=a/MINALLOC;
	for(int i=0;i<NUMALLOC;i++){
		int o=0;
		if(i>NUMALLOC-a){
			break;
		}
		for(int j=i;j<i+a;j++){
			if(buff[j]!=-1){
				o=1;
				break;
			}
		}
		if(o==0){
			for(int j=i;j<i+a;j++){
				buff[j]=a;
			}
			//printf("%d\n",a);
			return (char *)(ptr+(MINALLOC*i));
		}
	}
	return NULL;
}
void dealloc(char *a){
	int x = (void*)a-ptr;
	x=x/MINALLOC;
	int y = buff[x];
	for(int i=x;i<x+y;i++){
		buff[i]=-1;
	}
	return;
}
