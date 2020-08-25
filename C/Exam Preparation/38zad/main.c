#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc,char* argv[]){
	
	if(argc > 5){
		errx(1,"Wrong number of arguments");
	}

	if(strcmp(argv[1],"-d")==0){
		if(argc != 3){
			errx(2,"Wrong number of arguments");
		}

		char a;
		while((read(0,&a,sizeof(a)))==1){
			if(strchr(argv[2],a) == NULL){
				write(1,&a,sizeof(a));
			}
		}

	}
	else if(strcmp(argv[1],"-s")==0){
		if(argc != 3){
                        errx(2,"Wrong number of arguments");
                }
		
		char a;
		char first=1;
		while((read(0,&a,sizeof(a)))==1){
			char prev='\0';
			if(a!=prev){
				first=1;
			}
			if(strchr(argv[2],a)==NULL){
				if(!first){
					continue;
				}
				write(1,&a,sizeof(a));
				first=0;
			}
		}
	}
	else{
		if(strlen(argv[1]) != strlen(argv[2])){
			errx(1,"err");
		}

		char a;
		char* set1=argv[1];
		while((read(0,&a,sizeof(a)))==1){
			char *at = strchr(argv[1],a);
			if(at){
				int index=(int)(at-set1);	
				printf("%c", argv[2][index]);
			}
			else {
				printf("%c", a);	
			}

		}
	}
	exit(0);
}
