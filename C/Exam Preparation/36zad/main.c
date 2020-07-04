/* TODO Check write/read */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
int main(int argc,char* argv[]){
	if(strcmp(argv[1],"-n")==0){
		int line=1;
		char a;
		int fd;
		if(argc==2){
			printf("%d ",line++);
        	        fflush(NULL);
			while(read(0,&a,sizeof(a))>0){
                        	write(1,&a,sizeof(a));
				if(a=='\n'){
                                        printf("%d ",line++);
					fflush(NULL);
                                }

                        }
	
		}
		else{
			printf("%d ",line++);
               		fflush(NULL);

			for(int i=2;i<argc;i++){
				if(strcmp(argv[i],"-")==0){
					while(read(0,&a,sizeof(a))>0){         
                                		write(1,&a,sizeof(a));
						if(a=='\n'){
                                      		 	printf("%d ",line++);
                                       			fflush(NULL);
                               			 }
                         
					}
					continue;	

				}
				if((fd=open(argv[i],O_RDONLY))==-1){
					err(1,"Error opening file %s",argv[i]);
				}

				while(read(fd,&a,sizeof(a))>0){		
					write(1,&a,sizeof(a));
					if(a=='\n'){
                                        	printf("%d ",line++);
						fflush(NULL);
                        	       	 }
				}
			}
		}
	}
	else{
		char a;
                int fd;

		if(argc==1){
                        while(read(0,&a,sizeof(a))>0){
                                write(1,&a,sizeof(a));
                        }
                }

		for(int i=1;i<argc;i++){
                        if(strcmp(argv[i],"-")==0){
                                while(read(0,&a,sizeof(a))>0){         
                                        write(1,&a,sizeof(a));
                                }
                                continue;       
			}
                        
                        if((fd=open(argv[i],O_RDONLY))==-1){
                                err(1,"Error opening file %s",argv[i]);
                        }

                        while(read(fd,&a,sizeof(a))>0){         
                                write(1,&a,sizeof(a));
		      	}


		}
	}
}

		

