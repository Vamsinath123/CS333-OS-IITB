#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

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


int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;

	FILE* fp;
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp == NULL) {
			printf("File doesn't exists.");
			return -1;
		}
	}

	while(1) {			
		/* BEGIN: TAKING INPUT */
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
		//printf("Command entered: %s (remove this debug output later)\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
   
       //do whatever you want with the commands, here we just print them

		/*for(i=0;tokens[i]!=NULL;i++){
			printf("found token %s (remove this debug output later)\n", tokens[i]);
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
					int rc_wait = wait(NULL);	
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
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);
	}
	return 0;
}
