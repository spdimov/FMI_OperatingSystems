/* Write a C program, which accepts parameter- name of a binary file with uint32_t numbers. Program should sort the file. Limits: max 100 000 000 numbers,256 RAM, 8GB free disc space */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>


int cmp(const void *a, const void *b){
//	return ( *(uint32_t*)a - *(uint32_t*)b);
	if(*((uint32_t*)a) > *((uint32_t*)b) ){
		return 1;
	}
	else{
		return -1;
	}
}

int main(int argc,char* argv[]){

	if(argc!=2){
		errx(1,"Wrong arguments");
	}

	int fd1;
	if((fd1=open(argv[1],O_RDONLY))==-1){
		err(2,"Error opening file");
	}
	
	int t1;
	if((t1=open("tmp1",O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR))==-1){
		const int old_errno=errno;
		close(fd1);
		errno=old_errno;
		err(3,"tmp1 failed");	
	}
	
	struct stat st;
	if(fstat(fd1,&st)<0){
		const int old_errno=errno;
		close(fd1);
		close(t1);
		errno=old_errno;
		err(5,"failed stat");
	}
	
	if(st.st_size % sizeof(uint32_t) != 0){
		const int old_errno=errno;
		close(fd1);
		close(t1);
		errno=old_errno;
		errx(6,"Inconsistent f");
	}
	
	int numel=st.st_size / sizeof(uint32_t);
	int lhalf=numel/2;
	uint32_t *p=malloc((lhalf)*sizeof(uint32_t));
	if(!p){
		const int old_errno=errno;
		close(fd1);
		close(t1);
		errno=old_errno;
		err(7,"Error malloc");
	}
	
	size_t res=read(fd1,p,lhalf*sizeof(uint32_t));
	if(res!=(ssize_t)(lhalf*sizeof(uint32_t))){
		const int old_errno=errno;
		close(fd1);
		free(p);
		errno=old_errno;
		err(8,"Error reading file");
	}
	qsort(p,lhalf,sizeof(uint32_t),cmp);
	
	res=write(t1,p,lhalf*sizeof(uint32_t));
	if(res != (ssize_t)(lhalf*sizeof(uint32_t))){
		const int old_errno=errno;
		close(fd1);
		close(t1);
		free(p);
		errno=old_errno;
		err(9,"Error writing file");
	}

	free(p);
	int rhalf=numel-lhalf;
	uint32_t *rp=malloc(rhalf*sizeof(uint32_t));
	if(!rp){
                const int old_errno=errno;
                close(fd1);
                errno=old_errno;
                err(10,"Error malloc");
        }

	res=read(fd1,rp,rhalf*sizeof(uint32_t));
	if(res != (ssize_t)(rhalf*sizeof(uint32_t))){
		const int old_errno=errno;
		close(fd1);
		close(t1);
		free(rp);
		errno=old_errno;
		err(11,"Error reading file");
	}
	int t2;
	if((t2=open("tmp2",O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR))==-1){
                const int old_errno=errno;
                close(fd1);
                close(t1);
                errno=old_errno;
                err(4,"tmp2 failed");
        }

	qsort(rp,rhalf,sizeof(uint32_t),cmp);
	res=write(t2,rp,rhalf*sizeof(uint32_t));
	if(res != (ssize_t)(rhalf*sizeof(uint32_t))){
		const int old_errno=errno;
                close(fd1);
                close(t1);
		close(t2);
                free(rp);
                errno=old_errno;
                err(12,"Error writing file");
	}

	free(rp);
	close(fd1);

	lseek(t1,0,SEEK_SET);
	lseek(t2,0,SEEK_SET);
	int fd2;
	if((fd2=open("f2",O_CREAT | O_TRUNC | O_RDWR,S_IRUSR | S_IWUSR))==-1){
		const int old_errno=errno;
		close(t1);
		close(t2);
		errno=old_errno;
		err(13,"Error opening file");
	}	
	
	uint32_t a;
	uint32_t b;

	while((read(t1,&a,sizeof(a)) == sizeof(a)) && (read(t2,&b,sizeof(b))==sizeof(b))) {
		if ( a <= b ) {
			write(fd2,&a,sizeof(a));
			lseek(t2,-1*sizeof(b),SEEK_CUR);	
		}
		else {
			write(fd2,&b,sizeof(b));
			lseek(t1,-1*sizeof(a),SEEK_CUR);
		}
	
	}

	while(read(t1,&a,sizeof(a))==sizeof(a)){
		write(fd2,&a,sizeof(a));
	}

	while(read(t2,&b,sizeof(b))==sizeof(b)){
                write(fd2,&b,sizeof(b));
        }
	
	close(t1);
	close(t2);
	exit(0);







}
