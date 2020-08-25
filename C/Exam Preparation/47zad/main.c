#include<err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

int main(int argc,char* argv[]){
	
	if(argc != 2){
		errx(1,"Wrong number of arguments. Usage %s <dirpath>",argv[0]);
	}

	struct stat st;
	if(stat(argv[1],&st)==-1){
		err(2,"Couldnt stat %s",argv[0]);
	}
	if(S_ISDIR(st.st_mode)==0){
		errx(3,"%s is not a valid directory",argv[1]);
	}

	int a[2];

	if(pipe(a)==-1){
		err(4,"Couldnt pipe a");
	}

	pid_t child_find=fork();
	if(child_find==-1){
		err(5,"Couldnt fork child_find");
	}

	if(child_find==0){
		close(a[0]);
		
		if(dup2(a[1],1)==-1){
			err(6,"Couldnt dup2");
		}

		if(execlp("find","find",argv[1],"-type","f",(char*)NULL)==-1){
		}
	}

	close(a[1]);

	int b[2];

	if(pipe(b)==-1){
		err(7,"Couldnt pipe b");
	}

	pid_t child_sort=fork();
	if(child_sort==-1){
		err(8,"Couldnt fork");
	}
	if(child_sort==0){
		close(b[0]);
		if(dup2(a[0],0)==-1){
			err(9,"Couldnt dup2");
		}
		if(dup2(b[1],1)==-1){
			err(10,"Couldnt dup2");
		}

	if(execlp("sort", "sort", "-nr", (char *)NULL) == -1){
			err(12, "could not execlp");
		}
	}
	close(b[1]);
	close(a[0]);
	
	int c[2];
	if(pipe(c)==-1){
		err(13,"Couldnt pipe c");
	}

	pid_t child_head=fork();

	if(child_head==-1){
		err(14,"Couldnt fork");
	}

	if(child_head==0){
		close(c[0]);
		dup2(c[1],1);
		dup2(b[0],0);

		if(execlp("head", "head", "-1", (char *)NULL) == -1)
			err(18, "Could not execlp");
	}
	close(c[1]);
	close(b[0]);

	if(dup2(c[0],0)==-1){
		err(19,"Couldnt dup2");
	}

	if(execlp("cut", "Pesho is cutting", "-d:", "-f2", (char *)NULL) == -1){
		err(20, "could not execlp cut with args");
	}

}
