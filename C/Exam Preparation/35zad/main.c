/* f1.bin-original file
 * f2.bin-edited file
 * f3.bin-all changes(struct containing position,original byte and new byte */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
struct struct_t{
	uint16_t pos;
	uint8_t org;
	uint8_t new;
};
int main(int argc, char* argv[]){
	
	if(argc!=4){
		errx(1,"Correct usage: ./main f1.bin f2.bin patch.bin");
	}

	struct stat st;
        if(stat(argv[1],&st)<0){
                err(2,"Error stat file %s",argv[1]);
        }
	int fd1_size=st.st_size;

        if(stat(argv[2],&st)<0){
                err(2,"Error stat file %s",argv[2]);
        }
	int fd2_size=st.st_size;
	
	if(fd1_size!=fd2_size){
		err(4,"Incorrect file sizes");
	}


	int fd1;
	if((fd1=open(argv[1],O_RDONLY))==-1){
		err(3,"Error opening file %s",argv[1]);
	}
	
	int fd2;
	if((fd2=open(argv[2],O_RDONLY))==-1){
		const int old_errno=errno;
		close(fd1);
		errno=old_errno;
		err(3,"Error opening file %s",argv[2]);
	}

	int fd3;
	if((fd3=open(argv[3],O_RDWR | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR))==-1){
		const int old_errno=errno;
		close(fd1);
		close(fd2);
		errno=old_errno;
		err(3,"Error opening file %s",argv[3]);
	}
	
	uint8_t a;
	uint8_t b;
	uint16_t pos=0;
	struct struct_t el={0,0,0};
	while(read(fd1,&a,sizeof(a))==sizeof(a) && read(fd2,&b,sizeof(b))==sizeof(b)){
		if(a!=b){
			el.pos=pos;
			el.org=a;
			el.new=b;
			int res;
			if((res=write(fd3,&el,sizeof(el)))!=sizeof(el)){
				const int old_errno=errno;
				close(fd1);
				close(fd2);
				close(fd3);
				errno=old_errno;
				err(5,"Error writing to file %s",argv[3]);
			}
		}
		pos++;
	}
	
	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);

}
