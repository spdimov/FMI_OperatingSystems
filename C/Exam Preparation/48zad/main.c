#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdio.h>


int main(int argc,char* argv[]){

	if(argc < 3){
		errx(1,"Wrong number of arguments");
	}
	
	int fd;
	if((fd=open("run.log",O_CREAT | O_TRUNC

	char* args[12];
	int i=2,cnt=0;

	for(;i<argc;i++){
		args[cnt++]=argv[i];
	}
	args[cnt]=NULL;

	int stop_cnt=0;
	while(stop_cnt < 2){
		int start=time(NULL);
		
		int pid=fork();
		if(pid==0){
			if(execvp(args[0],args) ==-1){
				err(2,"err");
			}
		}
		int status;
		int result;
		wait(&status);
		if(WIFEXITED(status)){
			result=WEXITSTATUS(status);
		}
		int end=time(NULL);
		if(result != 0){
			stop_cnt++;
		}
		printf("%d   %d    %d",start,end,result);
	}
}
