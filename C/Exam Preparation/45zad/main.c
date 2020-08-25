#include <err.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	
	int a[2];
	if(pipe(a)==-1){
		err(1,"Couldnt pipe a");
	}
	
	const int child_cut=fork();

	if(child_cut ==-1){
		err(2,"Couldnt fork");
	}

	if(child_cut == 0){
		close(a[0]);
		if(dup2(a[1],1)==-1){
			err(3,"Couldnt dup2");
		}
		if(execlp("cut","cut","-d:","-f7","/etc/passwd",(char*)NULL)==-1){
			err(4,"Couldnt exec cut");
		}
	}
	close(a[1]);

	int b[2];

	if(pipe(b)==-1){
		err(5,"Couldnt pipe b");
	}
	const int child_sort=fork();
	if(child_sort==-1){
		err(6,"Couldnt fork");
	}
	
	if(child_sort==0){
		if(dup2(a[0],0)==-1){
			err(7,"Couldnt dup2");
		}
		close(b[0]);

		if(dup2(b[1],1)==-1){
			err(8,"Couldnt dup2");
		}
		if(execlp("sort","sort",(char*)NULL)==-1){
			err(9,"Couldnt exec sort");
		}
	}
	close(b[1]);
	
	int c[2];
	if(pipe(c)==-1){
		err(10,"Couldnt pipe c");
	}

	const int child_uniq=fork();
	if(child_uniq==-1){
		err(11,"Couldnt fork");
	}

	if(child_uniq == 0){
		if(dup2(b[0],0)==-1){
			err(12,"Couldnt dup2");
		}
		close(c[0]);

		if(dup2(c[1],1)==-1){
			err(13,"Couldnt dup2");
		}

		if(execlp("uniq","uniq -c","-c",(char*)NULL)==-1){
			err(14,"Error exec uniq");
		}
	}
	close(c[1]);

	if(dup2(c[0],0)==-1){
		err(15,"Error dup2");
	}

	if(execlp("sort","sort","-n",(char*)NULL)==-1){
		err(16,"Couldnt exec sort");
	}
	exit(0);
}

