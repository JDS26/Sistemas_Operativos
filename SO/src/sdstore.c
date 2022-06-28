#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char const *argv[]){
    char string[999];
    char* str;
	
	mkfifo("client_para_server", 0666);
	
	int fifo_client_server=open("client_para_server",O_WRONLY);
	int fifo_server_client = open("server_para_client", O_RDONLY);

    str= (char*) malloc(sizeof(char)*argc);
	
	if (argc<2){
		sprintf(string, "info\n");
		
	}
	else if(*argv[1] == 's') {
		if (argc==2){
			sprintf(string, "status\n");
		}
	}
	else {
		if(*argv[1] == 'p'){

			sprintf(string,"procfile ");
			for(int i=2;i<argc;i++){
				char tmp[20];
				sprintf(tmp,"%s ",argv[i]);
				strcat(string,tmp);
			}
		}
	}

	write(fifo_client_server,string,strlen(string));	
	close(fifo_client_server);
	
	int bytesRead = 0;
    while((bytesRead = read(fifo_server_client, str, 1024)) > 0)
        write(1, str, bytesRead);
    close(fifo_server_client);
	
	return 0;
} 