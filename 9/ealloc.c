#include "ealloc.h"

#define NUMALLOC PAGESIZE/MINALLOC
int buff[4][NUMALLOC];
void* ptr[4];

void init_alloc(){
	for(int i=0;i<NUMALLOC;i++){
		for(int j=0;j<4;j++){
			buff[j][i]=-1;
		}
	}
	for(int i=0;i<4;i++){
		ptr[i]=NULL;
	}
	return;
}
void cleanup(){
	for(int i=0;i<NUMALLOC;i++){
		for(int j=0;j<4;j++){
			buff[j][i]=-1;
		}
	}
	return;	
}
char* alloc(int a){
	if(a%MINALLOC!=0){
		return NULL;
	}
	a=a/MINALLOC;
	int k=0;
	for(k=0;k<4;k++){
		if(ptr[k]==NULL){
			ptr[k] = mmap(0,PAGESIZE,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_ANON|MAP_PRIVATE,0,0);
		}
		for(int i=0;i<NUMALLOC;i++){
			int o=0;
			if(i>NUMALLOC-a){
				break;
			}
			for(int j=i;j<i+a;j++){
				if(buff[k][j]!=-1){
					o=1;
					break;
				}
			}
			if(o==0){
				for(int j=i;j<i+a;j++){
					buff[k][j]=a;
				}
				return (char *)(ptr[k]+(MINALLOC*i));
			}
		}
	}
}
void dealloc(char *a){
	int ch;
	int i;
	for(i=0;ptr[i]!=NULL && i<4;i++){
		ch = (void *)a-ptr[i];
		if(ch<PAGESIZE && ch>=0){
			break;
		}
	}
	ch/=MINALLOC;
	int y = buff[i][ch];
	for(int j=ch;j<ch+y;j++){
		buff[i][j]=-1;
	}
	return;
}
