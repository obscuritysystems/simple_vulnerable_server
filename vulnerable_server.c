#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

//prototype
void process(int);

//eroor message handle
void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
	 
		
     int server_sockfd;
	 int server_loopsockfd;
	 int port;
     int client_length; 
     int pid;

     struct sockaddr_in server_address, client_address;

     if (argc < 2) {
         fprintf(stderr,"ERROR: No port in args\n");
         exit(1);
     }

     server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

     if (server_sockfd < 0){
        error("ERROR: Could not opening socket");
	 }

     bzero((char *) &server_address, sizeof(server_address));

     port = atoi(argv[1]);
     server_address.sin_family = AF_INET;
     server_address.sin_addr.s_addr = INADDR_ANY;
     server_address.sin_port = htons(port);

     if (bind(server_sockfd, (struct sockaddr *) &server_address,sizeof(server_address)) < 0){
              error("ERROR on binding to port nubmer");
	 }

     listen(server_sockfd,5);
     client_length = sizeof(client_address);

     while(1){

         server_loopsockfd = accept(server_sockfd, (struct sockaddr *) &client_address, &client_length);

        if (server_loopsockfd < 0){
             error("ERROR: On accept");
		}
        pid = fork();
        if (pid < 0){
             error("ERROR on fork");
		}
        if (pid == 0)  {
             close(server_sockfd);
             process(server_loopsockfd);
             exit(0);
        }
        else{ 
			close(server_loopsockfd);
		}
     } 

     return 0;
}

void process (int sock){

	int n;
	char buffer[256];

	bzero(buffer,256);
	n = read(sock,buffer,255);

	//this is a basic buffer overflow
	char badidea[100];
	bzero(badidea,100);
	strcpy(badidea,buffer);

	if (n < 0){
   		error("ERROR: Reading from socket");
	}
	n = write(sock,"I got your message dawg \n",25);
	n = write(sock,badidea,100);
	n = write(sock,"\n",1);
	
	if (n < 0){ 
		error("ERROR: Writing to socket");
	}
}

