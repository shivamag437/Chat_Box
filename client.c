#include<stdio.h>
#include<stdlib.h>

#include<sys/types.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>

#include<netinet/in.h>

int main(int argc, char** argv)
{
	int alice,bob;

	char *username;
	username=(char*)malloc(21*sizeof(char));

	char *connection_name;
	connection_name=(char*)malloc(21*sizeof(char));

	alice = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_alice;
	server_alice.sin_family=AF_INET;
	server_alice.sin_port=htons(atoi(argv[2]));
	server_alice.sin_addr.s_addr=inet_addr(argv[1]);
	bzero(&server_alice.sin_zero, 8);

	if(connect(alice, (struct sockaddr*)&server_alice, sizeof(struct sockaddr_in)) == -1){
		printf("Connection could not be established!");
		exit(-1);
	}
	
	printf("Enter a temporary username (Not more than 20 characters): ");
	scanf("%s",username);

	//name exchange		
	recv(alice, connection_name, 21, 0);
	send(alice, username, strlen(username), 0);

	printf("Connected to %s.....\n",connection_name);

	char *buffer;
	buffer=(char*)malloc(350*sizeof(char));
	while(1){
		bzero(buffer, sizeof(buffer));

		printf("%s: ", username);
		scanf("%s",buffer);
		send(alice, buffer, strlen(buffer), 0);
		if((strncmp(buffer,"exit",4)) == 0){
			printf("Exiting....\n");
			break;
		}

		bzero(buffer, sizeof(buffer));

		recv(alice, buffer, 350, 0);
		printf("%s: %s\n", connection_name, buffer);
		if((strncmp(buffer,"exit",4)) == 0){
			printf("Exiting....\n");
			break;
		}
	}
	close(alice);
}
