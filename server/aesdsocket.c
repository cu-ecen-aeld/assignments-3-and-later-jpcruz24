#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

#define MAXDATASIZE 128 // max number of bytes we can get at once 
#define BACKLOG 10  

int socket_fd = 0;
volatile sig_atomic_t terminate_flag = false;

/*Server socket application*/
int main(int arg, char *arg2[]);

void daemonize() {
	int pid = fork();

	if (pid < 0) {
		perror("fork failed\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) {
		printf("---- daemon process\n");
		setsid();
	}
	else {
		exit(0);
	}
}

void signal_handler(int sig) {
	if ((sig == SIGINT) || (sig == SIGTERM)) 
	{
		unlink("/var/tmp/aesdsocketdata");
		terminate_flag = true; 
		//exit(-1);
	}
}


int main(int arg, char *arg2[]) {
	int status = 0;
	struct addrinfo hints, *servinfo, *p;
	//nt socket_bind = 0;
	int new_fd = 0;
	int fd;
	int yes = 1;
	int recv_data = 0;
	char data_rx[MAXDATASIZE]={0};
	char *outputfile = "/var/tmp/aesdsocketdata";
	char *arg_rc = "-d";
	char ip_str[INET_ADDRSTRLEN];
	struct sockaddr_storage their_addr;
	struct sigaction sa;

	socklen_t addr_size;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = signal_handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);

	if ((status = getaddrinfo(NULL, "9000", &hints, &servinfo)) != 0) {
		syslog(LOG_ERR, "Socket Error getting info");
		exit(-1);
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			close(socket_fd);
			syslog(LOG_ERR, "Socket Creation Error ");
			continue;
		}

		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
			close(socket_fd);
			syslog(LOG_ERR, "Socket bind Error ");
			continue;
		}
		break;
	}


	freeaddrinfo(servinfo);
	if(listen(socket_fd, 5)< 0)
	{
		close(socket_fd);
		syslog(LOG_ERR, "Socket listen Error ");
		exit(-1);
	}
	if(arg == 2)
	{
		if(strcmp(arg_rc, arg2[1]) == 0)
		{
			daemonize();	
		}
	}
	printf("server: waiting for connections...\n");
	while(!terminate_flag)
	{
		addr_size = sizeof their_addr;
		new_fd = accept(socket_fd, (struct sockaddr *)&their_addr, &addr_size);

		if (new_fd == -1) {
			close(new_fd);
			syslog(LOG_ERR, "Socket Fail to accept connection ");
			exit(-1);
		}
		inet_ntop(AF_INET, &(their_addr), ip_str, INET_ADDRSTRLEN);
		syslog(LOG_INFO, "Accepted connection from %s", ip_str);
		printf("Accepted connection from %s", ip_str);


		fd = open(outputfile , O_RDWR | O_CREAT | O_APPEND, 0644 );
		if (fd == -1)
		{
			syslog(LOG_ERR, "Socket Fail to receive data: ");
			close(new_fd);
		}
		while ((recv_data = recv(new_fd, data_rx, sizeof(data_rx), 0)) > 0 )
		{

			//printf("\n-----------> # Bytes to write: %d \n", recv_data);

			int write_stat = write(fd, data_rx, recv_data);

			if(write_stat == -1)
			{
				syslog(LOG_ERR, "Writing process fail: ");
				close(fd);
			}
			else
			{
				if(memchr(data_rx, '\n', recv_data) != NULL)
				{
					lseek(fd, 0, SEEK_SET);
					while ((recv_data = read(fd, data_rx, sizeof(data_rx))) > 0) {
						send(new_fd, data_rx, recv_data, 0);
					}
				}
			}
		}
		close(fd);
		close(new_fd);
		syslog(LOG_INFO, "Closed connection from %s", ip_str);
	}
	close(socket_fd);


	syslog(LOG_INFO, "Caught signal, exiting");
	return 0;
}
