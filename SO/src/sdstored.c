#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

int nop = 1;
int bcompress = 1;
int bdecompress = 1;
int gcompress = 1;
int gdecompress = 1;
int encrypt = 1;
int decrypt = 1;

//--------------------------------------

int nop_count = 0;
int bcompress_count = 0;
int bdecompress_count = 0;
int gcompress_count = 0;
int gdecompress_count = 0;
int encrypt_count = 0;
int decrypt_count = 0;

void sigint_handler(int s){
	printf("\nEXITING SERVER\n");
	_exit(0);
}

void ler_fic(char* ficheiro){
	int fd;

	if((fd = open(ficheiro,O_RDONLY,0666))<0){
		perror("open");
	}
	
	char* buf = calloc(512, sizeof(char));
	read(fd, buf, 512);
	
	char* trans=strtok(buf,"\n");
	int lentrans = 0;
	char *inter[7];
	char *final[7];
	int j=0;
	while (trans != NULL){
		inter[j]=trans;
		j++;
		trans=strtok(NULL,"\n");
		lentrans++;
	}

	int len;
	for(int i=0; i<7;i++){
		char* transv2=strtok(inter[i]," ");
		len=0;
		while (transv2 != NULL){
			len++;
			if (len%2==0){
				final[i]=transv2;
			}
			transv2=strtok(NULL," ");
		}
	}

	nop = atoi(final[0]);
	bcompress = atoi(final[1]);
	bdecompress = atoi(final[2]);
	gcompress = atoi(final[3]);
	gdecompress = atoi(final[4]);
	encrypt = atoi(final[5]);
	decrypt = atoi(final[6]);

}



void write_status(char* fifo){

	int fifo_server_client = open(fifo, O_WRONLY);
	//------------------------- nop
	char printnop[40];
	snprintf(printnop, 40, "Transform nop: %d/%d\n", nop_count,nop);
	write(fifo_server_client,printnop,strlen(printnop));

	//------------------------- bcompress
	char printbcompress[40];
	snprintf(printbcompress, 40, "Transform bcompress: %d/%d\n", bcompress_count,bcompress);
	write(fifo_server_client,printbcompress,strlen(printbcompress));
	
	//-------------------- bdecompress
	char printbdecompress[40];
	snprintf(printbdecompress, 40, "Transform bdecompress: %d/%d\n", bdecompress_count,bdecompress);
	write(fifo_server_client,printbdecompress,strlen(printbdecompress));
	
	//------------------ gcompress
	char printgcompress[40];
	snprintf(printgcompress, 40, "Transform gcompress: %d/%d\n", gcompress_count,gcompress);
	write(fifo_server_client,printgcompress,strlen(printgcompress));
	
	//--------------------------- gdecompress
	char printgdecompress[40];
	snprintf(printgdecompress, 40, "Transform gdecompress: %d/%d\n", gdecompress_count,gdecompress);
	write(fifo_server_client,printgdecompress,strlen(printgdecompress));

	//--------------------------- encrypt
	char printencrypt[40];
	snprintf(printencrypt, 40, "Transform encrypt: %d/%d\n", encrypt_count,encrypt);
	write(fifo_server_client,printencrypt,strlen(printencrypt));
	//--------------------------- decrypt
	char printdecrypt[40];
	snprintf(printdecrypt, 40, "Transform decrypt: %d/%d\n", decrypt_count,decrypt);
	write(fifo_server_client,printdecrypt,strlen(printdecrypt));
	
	//escrever pid principal do servidor
	char printpid[40];
	int pid= getpid();
	snprintf(printpid, 40, "pid: %d\n", pid);
	write(fifo_server_client,printpid,strlen(printpid));
	
	close(fifo_server_client);
}

void exec_transf(char* t,char folder[30]){
	const char * nopchar = "nop";
	const char * bcompresschar = "bcompress";
	const char * bdecompresschar = "bdecompress";
	const char * decryptchar = "decrypt";
	const char * encryptchar = "encrypt";
	const char * gcompresschar = "gcompress";
	const char * gdecompresschar = "gdecompress";

	//---------------- nop
	if (strncmp(t,nopchar,3)==0){		
		strcat(folder,nopchar);

		while(nop_count==nop){
			pause();
		}
		nop_count++;
		execl(folder, "nop", NULL);
	}
	//---------------- bcompress
	else if (strncmp(t,bcompresschar,9)==0){

		strcat(folder,bcompresschar);

		while(bcompress_count==bcompress){
			pause();
		}
		bcompress_count++;
		execl(folder, "bcompress", NULL);
	}
	//---------------- bdecompress
	else if (strncmp(t,bdecompresschar,11)==0){

		strcat(folder,bdecompresschar);

		while(bdecompress_count==bdecompress){
			pause();
		}
		bdecompress_count++;
		execl(folder, "bdecompress", NULL);
	}
	//---------------- decrypt 
	else if (strncmp(t,decryptchar,7)==0){

		strcat(folder,decryptchar);

		while(decrypt_count==decrypt){
			pause();
		}
		decrypt_count++;
		execl(folder, "decrypt", NULL);
	}
	//---------------- encrypt
	else if (strncmp(t,encryptchar,7)==0){

		strcat(folder,encryptchar);
		
		while(encrypt_count==encrypt){
			pause();
		}
		encrypt_count++;
		execl(folder, "encrypt", NULL);
	}
	//---------------- gcompress
	else if (strncmp(t,gcompresschar,9)==0){

		strcat(folder,gcompresschar);

		while(gcompress_count==gcompress){
			pause();
		}
		gcompress_count++;
		execl(folder, "gcompress", NULL);
	}
	//---------------- gdecompress
	else if (strncmp(t,gdecompresschar,11)==0){

		strcat(folder,gdecompresschar);

		while(gdecompress_count==gdecompress){
			pause();
		}
		gdecompress_count++;
		execl(folder, "gdecompress", NULL);
	}
}

int main(int argc, char const *argv[]){
	char file[50];
	char folder[50];
	
	if (argc ==3){
		strcpy(file,argv[1]);
		ler_fic(file);
		strcpy(folder,argv[2]);
	}
	else return 0;

	//--------------------------------------

	signal(SIGINT,sigint_handler);

    mkfifo("server_para_client", 0666);

	while (1){
		int fifo_client_server = open("client_para_server", O_RDONLY);
        int fifo_server_client = open("server_para_client", O_WRONLY);
		
		//----------------------------
		char* buf = calloc(512, sizeof(char));
		read(fifo_client_server, buf, 512);
		//----------------------------

		if (strncmp(buf, "info",4) == 0){
			char ajuda[150]= "./sdstore status\n./sdstore procfile input-filename output-filename Transform-id-1 Transform-id-2 ...\n";
			write(fifo_server_client,ajuda,150);
		}
		else if(strncmp(buf, "status",6) == 0) {
			write_status("server_para_client");
		}
		else if(strncmp(buf, "procfile",8) == 0) {
			char* trans;
			const char s[2] = " ";
			trans=strtok(buf,s);
			int lentrans= 0;
			int numtransf= 0;
			char *transformacoes[7];
			char *in_out[2];
			char *execute[9];
			
			int i=0;
			int j=0;
			while (trans != NULL){
				lentrans++;
				if (lentrans>3){
					transformacoes[i]=trans;
					execute[i+j+1]=trans;
					i++;
				}
				else if (lentrans>1){
					in_out[j]=trans;
					execute[j+1]=trans;
					j++;
				}
    			trans = strtok (NULL," ");
			}
			execute[i+j+1]=NULL;

			numtransf= lentrans-3;

			write(fifo_server_client,"Pending\n",8);
			
			char* file_in=in_out[0];
			char* file_out=in_out[1];
			
			char *transf_filho[numtransf];
		
			int fd1[numtransf-1][2];
			
			int in = open(file_in,O_RDWR,0666);
			int out = open(file_out, O_CREAT | O_RDWR | O_TRUNC, 0666);

			for (int i=0;i<numtransf;i++){

				transf_filho[i]=transformacoes[i];

				if (numtransf>1){
					if ((pipe(fd1[i]))<0){
						perror("pipe");
						_exit(i);
					}
				}

				if(!fork()){
					if (numtransf==1){
						dup2(in,0);
						dup2(out,1);
						close(in);
						close(out);
						exec_transf(transformacoes[i],folder);
					}
					else{
						if (i==0){
							dup2(in,0);
							close(in);
							
							close(fd1[i][0]);
							dup2(fd1[i][1],1);
							close(fd1[i][1]);
							exec_transf(transformacoes[i],folder);

						}
						else if (i==numtransf-1){
							dup2(fd1[i-1][0],0);
							close(fd1[i-1][0]);
							
							dup2(out,1);
							close(out);

							exec_transf(transformacoes[i],folder);
								
						}
						else {
							dup2(fd1[i-1][0],0);
							close(fd1[i-1][0]);

							close(fd1[i][0]);

							dup2(fd1[i][1],1);
							close(fd1[i][1]);

							exec_transf(transformacoes[i],folder);
								
						}
					}
					_exit(i);
				}
				if (numtransf>1){
					
					if (i==0){
						close(fd1[i][1]);
					}
					else if (i==numtransf-1){
						close(fd1[i-1][0]);
					}
					else {
						close(fd1[i][1]);
						close(fd1[i-1][0]);
					}
				}
			}

			write(fifo_server_client,"Processing\n",11);

			int status;
			for(int i=0;i<numtransf;i++){
				int terminated_pid = wait(&status);
				int num_transform = WEXITSTATUS(status);
				if (transf_filho[num_transform]=="nop")nop_count--;
				else if (transf_filho[num_transform]=="bcompress")bcompress_count--;
				else if (transf_filho[num_transform]=="bdecompress")bdecompress_count--;
				else if (transf_filho[num_transform]=="decrypt")decrypt_count--;
				else if (transf_filho[num_transform]=="encrypt")encrypt_count--;
				else if (transf_filho[num_transform]=="gcompress")gcompress_count--;
				else if (transf_filho[num_transform]=="gdecompress")gdecompress_count--;
			}
		}
		free(buf);
    	close(fifo_server_client);
    	close(fifo_client_server);
	}
	return 0;
}