/*Binary files f1 and f2 contain 32bit numbers without a sign.File f1 contains n couples numbers.Lets the i-th couple is <xi,yi>.Write a C program, which gets the interval starting at xi with length yi from file f2 and write it in file f3 */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
int main(int argc,char* argv[]){
	
	if(argc != 4){
		errx(1,"Wrong arguments");
	}
	
	int fd1;
	int fd2;
	int fd3;
	
	if((fd1=open(argv[1],O_RDONLY))==-1){
		err(2,"Error reading %s",argv[1]);
	}

	if((fd2=open(argv[2],O_RDONLY))==-1){
		const int old_errno=errno;
		close(fd1);
		errno=old_errno;
                err(3,"Error reading %s",argv[2]);
        }
	
	if((fd3=open(argv[3],O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR))==-1){
		const int old_errno=errno;
		close(fd1);
		close(fd2);
		errno=old_errno;
                err(4,"Error reading %s",argv[3]);
        }
	
	struct stat st1;
	struct stat st2;
	
	if(fstat(fd1,&st1)==-1){
		const int old_errno=errno;
		close(fd1);
		close(fd2);
		close(fd3);
		errno=old_errno;
		err(5,"Error stat file %s",argv[1]);
	}

	if(fstat(fd2,&st2)==-1){
                const int old_errno=errno;
                close(fd1);
                close(fd2);
                close(fd3);
                errno=old_errno;
                err(6,"Error stat file %s",argv[2]);
        }
	
	struct pair_t{
                uint32_t first;
                uint32_t second;
        }pair;

	if(st1.st_size % sizeof(pair) != 0 || st2.st_size/2 % sizeof(pair) != 0){
		const int old_errno=errno;
		close(fd1);
		close(fd2);
		close(fd3);
		errno=old_errno;
		errx(7,"Error in files");
	}


	int read_size;
	while((read_size=read(fd1,&pair,sizeof(pair))) > 0){
		off_t check=lseek(fd2,pair[0]*sizeof(uint32_t),SEEK_SET);
		if(check < 0){
			const int old_errno=errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno=old_errno;
			err(8,"Error reading file");
		}
		uint32_t storage;
	        uint32_t cnt=0;
		while(read(fd2,&storage,sizeof(storage))){
			int wrt_size;
			if(( wrt_size=write(fd3,&storage,sizeof(storage)))!= sizeof(storage)){
				const int old_errno=errno;
                       		close(fd1);
                        	close(fd2);
                        	close(fd3);
                        	errno=old_errno;
				err(9,"Error writing to file");
			}
			cnt++;
			if(cnt==pair[1]){
				break;
			}
		}	
	
	}

	close(fd1);
	close(fd2);
	close(fd3);

	exit(0);
}
