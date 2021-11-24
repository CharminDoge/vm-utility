#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write


int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , read_size;
	struct sockaddr_in server , client;
	char client_message[2000];
	char passwd[2000] = "ciao";
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	
	//accept connection from an incoming client
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	puts("Connection accepted");
	
	int cond = 0;
	//Receive a message from client
	memset(client_message, 0, 2000*sizeof(char));
	while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
	{	
		cond = 0;
		//Send the message back to client
		for (int i = 0; i < 2000; i++){
			if (client_message[i] != passwd[i]){
				write(client_sock , "nope\n" , strlen("nope\n"));
				cond = 1;
				break;
			}
		}
		memset(client_message, 0, 2000*sizeof(char));
		if (cond == 0) {
			write(client_sock , "access granted fra\n" , strlen("access granted fra\n"));
			break;
		}
	}
	
	system("Start-VM Win11Giulia");
	
	
	if(read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
	
	return 0;
}