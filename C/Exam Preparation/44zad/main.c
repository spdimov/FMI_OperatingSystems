#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>


int main(){

	while(1){
		char cmd[128];
		ssize_t rd_size;
		if((rd_size=read(0,&cmd,sizeof(cmd)))==-1){
			err(1,"Error reading cmd");
		}
		cmd[rd_size-1]='\0';


		if(strcmp(cmd,"exit")==0){
			break;
		}

		const pid_t child=fork();

		if(child==-1){
			err(2,"Couldnt fork");
		}
		
		if(child==0){
			if(execlp(cmd,"grgr",(char*)NULL)==-1){
				err(3,"Error execling");
			}
		}
		
		if(wait(NULL)==-1){
			err(4,"Error in waiting");	
		}

	}	
	exit(0);
}
