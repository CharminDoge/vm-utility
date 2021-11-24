#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

#define GOOD 0
#define ERROR 1
#define EMPTY 2
#define TOOSHORT 3
#define TOOLONG 4

int debug_on = 0;


// la funzione prende una stringa di 3 byte in input
int get_line(char* inputStr, int length) {
    int chr, check;

    if (fgets (inputStr, length, stdin) == NULL){
      return EMPTY;
    }
    // controllo che almeno 1 carattere sia stato inserito in input
    int len = strlen(inputStr);
    if (len < 2){
      return TOOSHORT;
    }
    // controllo a fine linea se trovo \n
    if (inputStr[len - 1] != '\n') {
        check = 0;

        // flush
        while (((chr = getchar()) != '\n') && (chr != EOF))
            check = 1;

        if (check==1){
          return TOOLONG;
        }
        else
          return GOOD;
    }
    // termino l'array
    inputStr[len - 1] = '\0';
    return GOOD;
}

int main(int argc , char *argv[])
{
	int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];
	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	
	puts("Connected\n");
	
	//keep communicating with server
	while(1)
	{
		
		printf("Enter message : ");
		get_line(message, 1000);
		
		//Send some data
		if( send(sock , message , strlen(message) , 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
		
		//Receive a reply from the server
		if( recv(sock , server_reply , 2000 , 0) < 0)
		{
			puts("recv failed");
			break;
		}
		
		puts("Server reply :");
		puts(server_reply);
		memset(server_reply, 0, 2000*sizeof(char));
	}
	
	close(sock);
	return 0;
}