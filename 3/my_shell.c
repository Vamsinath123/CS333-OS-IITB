#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <setjmp.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

int fore[MAX_NUM_TOKENS];
int fcheck=0;
sigjmp_buf flat;

char **tokenize (char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      	token[tokenIndex] = '\0';
      	if (tokenIndex != 0){
			tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
			strcpy(tokens[tokenNo++], token);
			tokenIndex = 0;
		}
    }
    else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

void sig_han(int s){
	for(int i=0;i<MAX_NUM_TOKENS;i++){
		if(fore[i]!=-1){
			kill(fore[i],SIGINT);
			waitpid(fore[i],NULL,0);
			fore[i]=-1;
		}
	}
	fcheck = 0;
	siglongjmp(flat,1);
}

int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;
	char **tokens1 = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	int i;

	FILE* fp;
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp == NULL) {
			printf("File doesn't exists.");
			return -1;
		}
	}
	pid_t waitlist[MAX_NUM_TOKENS];
	for(int i=0;i<MAX_NUM_TOKENS;i++){
		waitlist[i]=-1;
		fore[i]=-1;
	}
	int num = 0;
	signal(SIGINT,sig_han);
	if(sigsetjmp(flat,1)){
		printf("\n");
	}
	while(1) {			
		/* BEGIN: TAKING INPUT */
		tokens1[0]=NULL;
		bzero(line, sizeof(line));
		if(argc == 2) { // batch mode
			if(fgets(line, sizeof(line), fp) == NULL) { // file reading finished
				break;	
			}
			line[strlen(line) - 1] = '\0';
		} else { // interactive mode
			printf("$ ");
			scanf("%[^\n]", line);
			getchar();
		}

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

   		if(tokens[0]!=NULL && strcmp(tokens[0],"exit")==0){
   			for(i=0;i<num;i++){
				if(waitlist[i]!=-1){
					kill(waitlist[i],SIGKILL);
					waitpid(waitlist[i],NULL,0);
				}
			}
			free(tokens1);
			for(i=0;tokens[i]!=NULL;i++){
				free(tokens[i]);
			}
			free(tokens);
			break;
   		}
   		else{
   			int a=0;
	   		for(i=0;tokens[i]!=NULL;i++){
	   			if(strcmp(tokens[i],"&")==0){
	   				a=1;
	   			}
	   			else if(strcmp(tokens[i],"&&")==0){
	   				a=2;
	   			}
	   			else if(strcmp(tokens[i],"&&&")==0){
	   				a=3;
	   			}
	   		}
			if(a==0){
				/*for(int j=0;tokens[j]!=NULL;j++){
					printf("%s\n", tokens[j]);
				}*/
				if(tokens[0]!=NULL){
					int comp = strcmp(tokens[0],"cd");
					if(comp!=0){
						int rc = fork();
						if (rc < 0) {// fork failed; exit
							fprintf(stderr, "fork failed\n");
							exit(1);
						}
						else if (rc == 0) {
							//signal(SIGINT,dfl);
							int fa = execvp(tokens[0],tokens);
							if(fa == -1){
								printf("Shell: Incorrect command\n");
								int pid = getpid();
								for(i=0;tokens[i]!=NULL;i++){
									free(tokens[i]);
								}
								free(tokens);
								kill(pid, SIGKILL);
							}
						}
						else{
							// Freeing the allocated memory
							fore[0]=rc;
							int rc_wait = waitpid(rc,NULL,0);	
						}
					}
					else{
						if(tokens[2]==NULL){
							if(chdir(tokens[1])!=0){
								printf("Shell: Incorrect command\n");
							}
						}
						else{
							printf("Shell: Incorrect command\n");
						}
					}
				}
			}
			else if(a==1){
				/*for(int j=0;tokens[j]!=NULL;j++){
					printf("%s\n", tokens[j]);
				}*/
				for(i=0;strcmp(tokens[i],"&")!=0;i++){
					tokens1[i] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
					strcpy(tokens1[i], tokens[i]);
				}
				tokens1[i]=NULL;
				if(tokens1[0]!=NULL){
					int comp = strcmp(tokens1[0],"cd");
					if(comp!=0){
						int rc = fork();
						if (rc < 0) {// fork failed; exit
							fprintf(stderr, "fork failed\n");
							exit(1);
						}
						else if (rc == 0) {
							signal(SIGINT,SIG_IGN);
							int fa = execvp(tokens1[0],tokens1);
							if(fa == -1){
								printf("Shell: Incorrect command\n");
								_exit(0);
							}
						}
						else{
							waitlist[num]=rc;
							num++;
						}
					}
					else{
						if(chdir(tokens1[1])!=0){
							printf("Shell: Incorrect command\n");
						}
					}
				}
			}
			else if(a==2){
				tokens1[0]=NULL;
				int ph=0;
				for(int i=0;tokens[i]!=NULL;i++){
					if(strcmp(tokens[i],"&&")==0){
						/*for(int j=0;tokens1[j]!=NULL;j++){
							printf("%s\n", tokens1[j]);
						}*/
						if(tokens1[0]!=NULL){
							int comp = strcmp(tokens1[0],"cd");
							if(comp!=0){
								int rc = fork();
								if (rc < 0) {// fork failed; exit
									fprintf(stderr, "fork failed\n");
									exit(1);
								}
								else if (rc == 0) {
									int fa = execvp(tokens1[0],tokens1);
									if(fa == -1){
										printf("Shell: Incorrect command\n");
										int pid = getpid();
										for(int j=0;tokens1[j]!=NULL;j++){
											free(tokens1[j]);
										}
										free(tokens1);
										kill(pid, SIGKILL);
									}
								}
								else{
									// Freeing the allocated memory
									fore[0]=rc;
									int rc_wait = waitpid(rc,NULL,0);
								}
							}
							else{
								if(chdir(tokens1[1])!=0){
									printf("Shell: Incorrect command\n");
								}
							}
							for(int j=0;tokens1[j]!=NULL;j++){
								free(tokens1[j]);
							}
							ph=0;
							tokens1[0]=NULL;
						}
					}
					else{
						tokens1[ph] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
						strcpy(tokens1[ph], tokens[i]);
						ph++;
						tokens1[ph]=NULL;
					}
				}
				if(tokens1[0]!=NULL){
					int comp = strcmp(tokens1[0],"cd");
					if(comp!=0){
						int rc = fork();
						if (rc < 0) {// fork failed; exit
							fprintf(stderr, "fork failed\n");
							exit(1);
						}
						else if (rc == 0) {
							int fa = execvp(tokens1[0],tokens1);
							if(fa == -1){
								printf("Shell: Incorrect command\n");
								int pid = getpid();
								for(i=0;tokens1[i]!=NULL;i++){
									free(tokens1[i]);
								}
								free(tokens1);
								kill(pid, SIGKILL);
							}
						}
						else{
							// Freeing the allocated memory
							fore[0]=rc;
							int rc_wait = waitpid(rc,NULL,0);
							ph=0;	
						}
					}
					else{
						if(chdir(tokens1[1])!=0){
							printf("Shell: Incorrect command\n");
						}
						ph = 0;
					}
				}
			}
			else{
				tokens1[0]=NULL;
				int ph=0,po=0;
				int wai[MAX_TOKEN_SIZE];
				for(int i=0;i<MAX_TOKEN_SIZE;i++){
					wai[i]=-1;
				}
				for(int i=0;tokens[i]!=NULL;i++){
					if(strcmp(tokens[i],"&&&")==0){
						/*for(int j=0;tokens1[j]!=NULL;j++){
							printf("%s\n", tokens1[j]);
						}*/
						if(tokens1[0]!=NULL){
							int comp = strcmp(tokens1[0],"cd");
							if(comp!=0){
								int rc = fork();
								if (rc < 0) {// fork failed; exit
									fprintf(stderr, "fork failed\n");
									exit(1);
								}
								else if (rc == 0) {
									int fa = execvp(tokens1[0],tokens1);
									if(fa == -1){
										printf("Shell: Incorrect command\n");
										int pid = getpid();
										for(int j=0;tokens1[j]!=NULL;j++){
											free(tokens1[j]);
										}
										free(tokens1);
										kill(pid, SIGKILL);
									}
								}
								else{
									wai[po]=rc;
									po++;
									fore[fcheck]=rc;
									fcheck++;
									// Freeing the allocated memory
								}
							}
							else{
								if(chdir(tokens1[1])!=0){
									printf("Shell: Incorrect command\n");
								}
							}
							for(int j=0;tokens1[j]!=NULL;j++){
								free(tokens1[j]);
							}
							ph=0;
							tokens1[0]=NULL;
						}
					}
					else{
						tokens1[ph] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
						strcpy(tokens1[ph], tokens[i]);
						ph++;
						tokens1[ph]=NULL;
					}
				}
				if(tokens1[0]!=NULL){
					int comp = strcmp(tokens1[0],"cd");
					if(comp!=0){
						int rc = fork();
						if (rc < 0) {// fork failed; exit
							fprintf(stderr, "fork failed\n");
							exit(1);
						}
						else if (rc == 0) {
							int fa = execvp(tokens1[0],tokens1);
							if(fa == -1){
								printf("Shell: Incorrect command\n");
								int pid = getpid();
								for(i=0;tokens1[i]!=NULL;i++){
									free(tokens1[i]);
								}
								free(tokens1);
								kill(pid, SIGKILL);
							}
						}
						else{
							// Freeing the allocated memory
							wai[po]=rc;
							po++;
							fore[fcheck]=rc;
							rc++;
							ph=0;	
						}
					}
					else{
						if(chdir(tokens1[1])!=0){
							printf("Shell: Incorrect command\n");
						}
						ph = 0;
					}
				}
				for(int i=0;i<MAX_TOKEN_SIZE;i++){
					if(wai[i]!=-1){
						waitpid(wai[i],NULL,0);
					}
				}
			}
			for(i=0;i<num;i++){
				if(waitlist[i]!=-1){
					pid_t wa = waitpid(waitlist[i],NULL,WNOHANG);
					if(wa<0 || wa==waitlist[i]){
						waitpid(waitlist[i],NULL,0);
						printf("Shell: Background process finished\n");
						waitlist[i]=-1;
					}
				}
			}
			for(int j=0;tokens1[j]!=NULL;j++){
				free(tokens1[j]);
			}
			for(i=0;tokens[i]!=NULL;i++){
				free(tokens[i]);
			}
			free(tokens);
			for(int i=0;i<MAX_TOKEN_SIZE;i++){
				fore[i]=-1;
			}
			fcheck=0;
			
		}
	}
	return 0;
}
