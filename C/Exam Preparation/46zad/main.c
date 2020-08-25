#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>


void exec_cmd(char args[][5],char* cmd){
	int pid=fork();
	if(pid==0){
		char *execv_args[4];
		execv_args[0]=cmd;

		int i=1;
		int j=0;

		while(args[j][0] != '\0'){
			execv_args[i++]=args[j++];
		}
		execv_args[i]=(char*)NULL;
		if(execvp(cmd,execv_args)==-1){
			err(1,"Couldnt exec");
		}
	}
	wait(NULL);
}

int main(int argc,char* argv[]){
	
	char cmd[5];
	if(argc == 1){
		strcpy(cmd,"echo");
	}
	else if(argc==2 && strlen(cmd) < 5){
		strcpy(cmd,argv[1]);
	}
	else{
		errx(2,"Wrong number of arguments");
	}

	char arg[5];
	int index=0;
	int cnt=0;

	char cmd_args[3][5];
	while((read(0,arg+index,1)>0) && index <=4){
		if(cnt == 2){
			cmd_args[2][0]='\0';
			exec_cmd(cmd_args,cmd);
			cnt=0;
		}
		if(arg[index]=='\n' || arg[index]==' '){
			arg[index]='\0';
			strcpy(cmd_args[cnt++],arg);
			index=0;
		}
		else{
			index++;
		}
	}
	
}
