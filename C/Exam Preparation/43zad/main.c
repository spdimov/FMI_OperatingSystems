#include<err.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc,char* argv[]){
	
	if(argc != 2){
		errx(1,"Wrong number of arguments");
	}

	int fd1;
	if((fd1=open(argv[1],O_RDONLY))==-1){
		err(2,"Cannot open file to read");}
	}	

	int a[2];
	if(pipe(a)==-1){
		err(3,"Couldnt pipe a");
	}

	const pid_t child_cat=fork();
       	if(child_cat==-1){
		err(4,"Couldnt fork");
	}

	if(child_cat == 0){
		close(a[0]);
		if(dup2(a[1],1)==-1){
			err(5,"couldnt dup2");
		}
		if(execl("/bin/cat","cat",argv[1],(char*)NULL)==-1){
			err(6,"couldnt execl cat");
		}
	}
	close(a[1]);
	
	if(dup2(a[0],0)==-1){
		err(7,"Couldnt dup2");
	}
	
	if(execlp("sort","sort",(char*)NULL)==-1){
		err(8,"Couldnt execl sort");
	
	}
}
