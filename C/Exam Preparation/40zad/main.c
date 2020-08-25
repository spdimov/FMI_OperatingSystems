#include <string.h>
#include <stdint.h>
#include <err.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc,char* argv[]){

	if(argc < 3){
		errx(1,"Wrong number of arguments");
	}

	if(strcmp(argv[1],"-c")==0){
		if(argc != 3){
			errx(1,"Wrong number of arguments");
		}
		
		
		if(strchr(argv[2],'-')!=NULL){
			char a;
			ssize_t cnt=0;
			while(read(0,&a,sizeof(a))==sizeof(a)){
				cnt++;
				if(cnt>=(ssize_t)argv[2][0]-(ssize_t)'0' && cnt<=(ssize_t)argv[2][2]-(ssize_t)'0'){
					write(1,&a,sizeof(a));
				}
			}	
		}
		else{
			char a;
			ssize_t cnt=0;
			while(read(0,&a,sizeof(a))==sizeof(a)){
				cnt++;
				if(cnt==(ssize_t)argv[2][0]-(ssize_t)'0'){
					write(1,&a,sizeof(a));
				}
			}
		}
	}
	else if(strcmp(argv[1],"-d")==0 && strcmp(argv[3],"-f")==0){
		if(argc != 5){
                        errx(1,"Wrong number of arguments");
                }
		
		if(strchr(argv[4],'-')!=NULL){
			char a;
			ssize_t cnt=1;
			while(read(0,&a,sizeof(a))==sizeof(a)){
				if(cnt>=(ssize_t)argv[4][0]-(ssize_t)'0' && cnt<=(ssize_t)argv[4][2]-(ssize_t)'0'){
					if(cnt==(ssize_t)argv[4][2]-(ssize_t)'0' && a==argv[2][0]){
						
					}
					else{
						write(1,&a,sizeof(a));
					}
				}	
					
				if(a==argv[2][0]){
                                        cnt++;
                                }
				if(a=='\n'){
					cnt=1;
					write(1,"\n",1);
				}
			}
		
		}
		else{
			char a;
			ssize_t cnt=1;
			while(read(0,&a,sizeof(a))==sizeof(a)){
				if(cnt==(ssize_t)argv[4][0]-(ssize_t)'0' && a!=argv[2][0]){
					write(1,&a,sizeof(a));
				}
				if(a==argv[2][0]){
                                        cnt++;
                                }

			}
		}

	}
}
