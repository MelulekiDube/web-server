#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h> // this is for sockaddr_in
#include <string.h>
#include <pthread.h>

#define handle_error(msg)   \
	do                      \
	{                       \
		perror(msg);        \
		exit(EXIT_FAILURE); \
	} while (0)

int server_fd, new_clientsoc;
struct sockaddr_in address;
const int PORT = 8080; //Where the clients can reach at

void set_sockin_address()
{
	//set address to 0
	memset((char *)&address, 0, sizeof(address)); /* Clear structure */
	/*set address values*/
	/* htonl converts a long integer (e.g. address) to a network representation */
	/* htons converts a short integer (e.g. port) to a network representation */
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = INADDR_ANY;
}

//step 1: create socket
int create_socket()
{
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{ //if the socket return a number less than 0 an error occured
		handle_error("cannot create socket\n");
		return 0;
	}
	// socket was created
	printf("Socket created and the file descriptor is: %d\n", server_fd);
	return 1;
}

//step 2: name the socket by binding it to a port
int name_socket()
{
	set_sockin_address();
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		handle_error("bind failed");
		return 0;
	}
	// bind was successful
	printf("Successfully binded to port: %d\n", PORT);
	return 1;
}

void *rec_request(void *argp)
{
	int *nc_ptr = (int*) argp;
	int new_clientsoc = *nc_ptr;
	// step 4: Send and receive messages
	char buffer[1024] = {0};
	int valread = read(new_clientsoc, buffer, 1024);
	printf("%s\n", buffer);
	if (valread < 0)
	{
		printf("No bytes are there to read");
	}
	char *hello = "Hello from the server"; //IMPORTANT! WE WILL GET TO IT
	write(new_clientsoc, hello, strlen(hello));
	//step 5: Close the socket
	close(new_clientsoc);
	return NULL;
}

int main()
{
	int addrlen = sizeof(address);
	if (!(create_socket() && name_socket()))
	{
		return 0;
	}

	//step 3: Server listening waiting for connection request and accepts connection
	if (listen(server_fd, 3) < 0)
	{
		handle_error("In listen");
		exit(EXIT_FAILURE);
	}
	pthread_t tid;

	while (1)
	{
		new_clientsoc = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)(&addrlen));
		if (new_clientsoc < 0)
		{
			handle_error("In accept");
		}
		else
		{
			if(pthread_create(&tid, NULL, &rec_request,  &new_clientsoc))
				(*rec_request)((void*)&new_clientsoc);
		}
	}

	pthread_exit(NULL);
	return 0;
}