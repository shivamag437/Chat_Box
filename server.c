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
	int bob,alice,len;
	
	char *username;
	username=(char*)malloc(21*sizeof(char));

	char *connection_name;
	connection_name=(char*)malloc(21*sizeof(char));

	alice = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_alice;
	struct sockaddr_in server_bob;
	server_alice.sin_family=AF_INET;
	server_alice.sin_port=htons(atoi(argv[1]));
	server_alice.sin_addr.s_addr=INADDR_ANY;
	bzero(&server_alice.sin_zero, 8);

	if((bind(alice, (struct sockaddr*)&server_alice, sizeof(struct sockaddr_in))) == -1){
		printf("Server binding unsuccessful!\n");
		exit(-1);
	}

	printf("Enter a temporary username (Not more than 20 characters): ");
	scanf("%s",username);

	if((listen(alice, 2)) == -1){
		printf("Listen unsuccessful!\n");
		exit(-1);
	}
	else
		printf("%s is listening....\n", username);

	len=sizeof(struct sockaddr_in);

	if((bob=accept(alice, (struct sockaddr *)&server_bob, &len)) < 0){
		printf("Unable to accept connection!\n");
		exit(-1);
	}
	
	//name exchange	
	send(bob, username, strlen(username), 0);
	recv(bob, connection_name, 21, 0);
	
	printf("%s has accepted a connection from %s...\n", username, connection_name);

	char *buffer;
	buffer=(char*)malloc(350*sizeof(char));
	while(1){
		bzero(buffer, strlen(buffer));

		recv(bob, buffer, 350, 0);
		printf("%s: %s\n", connection_name, buffer);
		if((strncmp(buffer,"exit",4)) == 0){
			printf("Exiting....\n");
			break;
		}

		bzero(buffer, strlen(buffer));
		
		printf("%s: ", username);
		scanf("%s",buffer);
		send(bob, buffer, strlen(buffer), 0);
		
		if((strncmp(buffer,"exit",4)) == 0){
			printf("Exiting....\n");
			break;
		}
	}
	close(bob);
}
