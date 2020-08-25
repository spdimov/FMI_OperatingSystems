#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a,const void *b){
	if((*(uint32_t*)a) > (*(uint32_t*)b)){
		return 1;
	}
	else if ((*(uint32_t*)a) > (*(uint32_t*)b)){
		return -1;
	}
	return 0;
}

int main(int argc,char* argv[]){

	if(argc != 3){
		errx(1,"Wrong number of arguments");
	}

	int fd1;
	if((fd1=open(argv[1],O_RDONLY))==-1){
		err(2,"Error opening file %s",argv[1]);
	}

	int fd2;
	if((fd2=open(argv[2],O_CREAT | O_TRUNC | O_RDWR,S_IRUSR | S_IWUSR))==-1){
		const int old_errno=errno;
		close(fd1);
		errno=old_errno;
		err(3,"Error creating file %s",argv[2]);
	}
	
	struct stat st;
	
	if(fstat(fd1,&st)==-1){
		const int old_errno=errno;
                close(fd1);
		close(fd2);
                errno=old_errno;
                err(4,"Error stat file %s",argv[1]);
	}

	if(st.st_size % sizeof(uint32_t) != 0){
		const int old_errno=errno;
                close(fd1);
		close(fd2);
                errno=old_errno;
                errx(5,"Inconsistent file %s",argv[1]);
	}

	ssize_t numel=st.st_size / sizeof(uint32_t);
	ssize_t lhalf=numel/2;
	uint32_t *p=malloc(lhalf*sizeof(uint32_t));
	if(!p){
		printf("dsadada");
	}

	ssize_t rd;
        if((rd=read(fd1,p,lhalf*sizeof(uint32_t)))!=(ssize_t)(lhalf*sizeof(uint32_t))){
		const int old_errno=errno;
                close(fd1);
                close(fd2);
                errno=old_errno;
                errx(6,"Error reading file %s",argv[1]);
	}

	qsort(p,lhalf,sizeof(uint32_t),cmp);
	
	int t1;
	if((t1=open("tmp1",O_CREAT | O_TRUNC | O_RDWR,S_IRUSR | S_IWUSR))==-1){
		const int old_errno=errno;
                close(fd1);
                close(fd2);
                errno=old_errno;
                errx(5,"Error creating tmp1");
	}

	ssize_t wr;
	if((wr=write(t1,p,lhalf*sizeof(uint32_t)))!=(ssize_t)(lhalf*sizeof(uint32_t))){
		const int old_errno=errno;
                close(fd1);
                close(fd2);
                errno=old_errno;
                errx(6,"Error writing to tmp1");
	}
	free(p);

	uint32_t rhalf=numel-lhalf;
	uint32_t *rp=malloc(rhalf*sizeof(uint32_t));
	if(!rp){
		printf("daffsd");
	}

        if((rd=read(fd1,rp,rhalf*sizeof(uint32_t)))!=(ssize_t)(rhalf*sizeof(uint32_t))){
                const int old_errno=errno;
                close(fd1);
                close(fd2);
                errno=old_errno;
                errx(6,"Error reading file %s",argv[1]);
        }

	int t2;
        if((t2=open("tmp2",O_CREAT | O_TRUNC | O_RDWR,S_IRUSR | S_IWUSR))==-1){
                const int old_errno=errno;
                close(fd1);
                close(fd2);
		close(t1);
                errno=old_errno;
                errx(7,"Error creating tmp2");
        }

        if((wr=write(t2,rp,rhalf*sizeof(uint32_t)))!=(ssize_t)(rhalf*sizeof(uint32_t))){
                const int old_errno=errno;
                close(fd1);
                close(fd2);
		close(t1);
		close(t2);
                errno=old_errno;
                errx(8,"Error writing to tmp12)");
        }
        free(rp);
	
	lseek(t1,0,SEEK_SET);
	lseek(t2,0,SEEK_SET);
	uint32_t a;
	uint32_t b;
	while((read(t1,&a,sizeof(a))==sizeof(a)) && (read(t2,&b,sizeof(b))==sizeof(b))){
		printf("%d--%d\n",a,b);
		if(a<=b){
			write(fd2,&a,sizeof(a));
			lseek(t2,-1*sizeof(b),SEEK_CUR);
		}
		else{
			write(fd2,&b,sizeof(a));
			lseek(t1,-1*sizeof(a),SEEK_CUR);
		}	
	}

	while(read(t1,&a,sizeof(a))==sizeof(a)){
		write(fd2,&a,sizeof(a));
	}

	while(read(t2,&b,sizeof(b))==sizeof(b)){
		write(fd2,&b,sizeof(b));
	}
	close(fd1);
	close(fd2);
	close(t1);
	close(t2);
	exit(0);

}

