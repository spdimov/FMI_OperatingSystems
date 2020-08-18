/* Write a C program, which accepts one parameter - name of a binary file with bytes. Program should sort the file*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b)
{
    return ( *(uint8_t*)a - *(uint8_t*)b );
}
int main(int argc,char* argv[]){
	
	if(argc!=2){
		errx(1,"Wrong arguments");
	}

	int fd1;
	if((fd1=open(argv[1],O_RDWR))==-1){
		err(2,"Error opening file");
	}
	struct stat st;
        if(fstat(fd1,&st) == -1){
		err(3,"Error in stat file");
	}
	
	const size_t buf_size=st.st_size;
        uint8_t buffer[buf_size];
        int read_size=0;

	if((read_size=read(fd1,&buffer,buf_size))==-1){
		const int old_errno=errno;
		close(fd1);
		errno=old_errno;
		err(3,"Error reading file");
	}

	qsort(buffer,buf_size,sizeof(uint8_t),cmp);

	lseek(fd1,0,SEEK_SET);
	ssize_t write_size=buf_size;

	if((write(fd1,&buffer,buf_size)) != write_size){
		err(4,"Error writing to file");
	}

	close(fd1);
	exit(0);
}
