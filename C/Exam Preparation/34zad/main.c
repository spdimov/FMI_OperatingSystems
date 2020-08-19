#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

struct struct_t{
	uint16_t pos;
	uint8_t len;
	uint8_t reserved;
};

int main(int argc,char* argv[]){

	if(argc != 5){
		errx(1,"Usage: ./main f1.dat f1.idx f2.dat f2.idx");
	}

	struct stat st;

	if (stat(argv[2],&st) < 0){	
		err(2,"Failed stat");
	}

	if(st.st_size > 0xFFFF){
		warnx("Size of %s is grater than 0xFFFF",argv[1]);
	}

	if(st.st_size % sizeof(uint32_t) != 0){
		errx(3,"Invalid size of file %s",argv[2]);
	}

	int fd1dat = open(argv[1],O_RDONLY);
	if(fd1dat==-1){
		err(4,"Failed to open file %s",argv[1]);
	}

	int fd1idx = open(argv[2],O_RDONLY);
        if(fd1idx==-1){
		const int old_errno=errno;
		close(fd1dat);
		errno=old_errno;
                err(5,"Failed to open file %s",argv[2]);
        }
	
	int fd2dat = open(argv[3],O_RDWR | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR);
        if(fd2dat==-1){
                const int old_errno=errno;
                close(fd1dat);
		close(fd1idx);
                errno=old_errno;
                err(6,"Failed to open file %s",argv[3]);
        }
	 
	int fd2idx = open(argv[4],O_RDWR | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR);
        if(fd2idx==-1){
                const int old_errno=errno;
                close(fd1dat);
                close(fd1idx);
		close(fd2dat);
                errno=old_errno;
                err(7,"Failed to open file %s",argv[4]);
        }
	
	struct struct_t el;
	struct struct_t y;
	y.pos=0;
	y.len=0;
	y.reserved=0;
	while(read(fd1idx, &el,sizeof(el)) == sizeof(el)){
		uint8_t a;
		if(lseek(fd1dat,el.pos ,SEEK_SET)==-1){
			err(8,"Cannot seek to %d position in %s",el.pos,argv[1]);
		}

		read(fd1dat,&a,sizeof(a));

		if(a>='A' && a<='Z'){
			y.len=el.len;
			write(fd2dat,&a,sizeof(a));
			for(int i=0;i<el.len;i++){
				read(fd1dat,&a,sizeof(a));
				write(fd2dat,&a,sizeof(a)); 
			}	
			write(fd2idx,&y,sizeof(y));
			y.pos+=y.len;
		}
	}
	close(fd1dat);
	close(f1idx);
	close(f2dat);
	close(f2idx);
	exit(0);

}
