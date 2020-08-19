#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
struct triple{
	uint16_t disp;
	uint8_t org;
	uint8_t new;
};
int main(int argc,char* argv[]){
	
	if(argc!=4){
		errx(1,"Usage: ./main patch.bin f1.bin f2.bin");
	}
	int patch;
	if((patch=open(argv[1],O_RDONLY))==-1){
		err(2,"Error opening file %s",argv[1]);
	}

	int f1;
	if((f1=open(argv[2],O_RDONLY))==-1){
		const int old_errno=errno;
		close(patch);
		errno=old_errno;
		err(3,"Error opening file %s",argv[2]);
	}
	int f2;
	if((f2=open(argv[3],O_RDWR | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR))==-1){
		const int old_errno=errno;
		close(patch);
		close(f1);
		errno=old_errno;
		err(4,"Error opening file %s",argv[3]);
	}

	struct stat st;
	if(fstat(patch,&st)==-1){
		const int old_errno=errno;
		close(patch);
		close(f1);
		close(f2);
		errno=old_errno;
		err(5,"Erorr stat file %s",argv[1]);
	}
	struct triple tr;
	if(st.st_size%sizeof(tr)!=0){
		const int old_errno=errno;
                close(patch);
                close(f1);
                close(f2);
                errno=old_errno;
		err(6,"Not correct file size %s",argv[1]);
	}
	uint8_t buff[2<<10];
	int rd_size;
	while((rd_size=read(f1,&buff,sizeof(buffer)))>0){
		int wr_size;
		if((wr_size=write(f2,&buff,sizeof(buffer)))!=rd_size){
			
		}
	}
	lseek(f1,0,SEEK_SET);
        lseek(f2,0,SEEK_SET);

	ssize_t rd;
	uint8_t a;
	uint8_t b;
	while((rd=read(patch,&tr,sizeof(tr)>0))){
		lseek(f1,tr.disp,SEEK_SET);
		lseek(f2,tr.disp,SEEK_SET);
		read(f1,&a,sizeof(a));
		if(a==tr.org){
			write(f2,&tr.new,sizeof(tr.new));
		}
	}
	

}
