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

#define MAXDATASIZE 20000 // max number of bytes we can get at once 
#define BACKLOG 10  

int socket_fd = 0;

/*Server socket application*/
int main(int arg, char *arg2[]);

void daemonize() {
    pid_t pid;
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }

    signal(SIGHUP, SIG_IGN);
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);
    if (chdir("/") < 0) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

void handle_sigint(int sig) {
	if (sig == SIGINT) 
	{
		printf("------------------>Interrupt signal received. Closing socket and exiting...\n");
		close(socket_fd);
		exit(-1);
	}
	else if(sig == SIGHUP)
	{
		syslog(LOG_INFO, "Daemon reloaded configuration");
	}
	else
	{
	
	}
}


int main(int arg, char *arg2[]) {
    int status = 0;
    struct addrinfo hints, *servinfo/*, *p*/;
    int socket_bind = 0;
    int fd;
	//int yes = 1;
    int recv_data = 0;
    char data_rcv[MAXDATASIZE]={0};
    char data_tx[MAXDATASIZE]={0};
    char *outputfile = "/var/tmp/aesdsocketdata";
    char ip_str[INET_ADDRSTRLEN];
    struct sockaddr_storage their_addr;
    //struct sockaddr_in client_addr;
    socklen_t addr_size;
    
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if(arg == 1)
	{
		if(strcmp(arg_rc, arg2[0]) == 0)
		{
			daemonize();	
		}
	}
	
	if ((signal(SIGINT, handle_sigint) == SIG_ERR) && (signal(SIGHUP, handle_sigint) == SIG_ERR)) {
		perror("SIGINT Register error");
		return 1;
	}
	
	if ((status = getaddrinfo(NULL, "9000", &hints, &servinfo)) != 0) {
		syslog(LOG_ERR, "Socket Error getting info");
		exit(-1);
	}
	
	socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (socket_fd == -1) 
	{
		close(socket_fd);
		syslog(LOG_ERR, "Socket Creation Error ");
		exit(-1);
	}

	socket_bind = bind(socket_fd, servinfo->ai_addr, servinfo->ai_addrlen);
	if (socket_bind == -1) {
		close(socket_fd);
		syslog(LOG_ERR, "Socket bind Error ");
		exit(-1);
	}
	
	freeaddrinfo(servinfo);
	if(listen(socket_fd, 5)< 0)
	{
		close(socket_fd);
		syslog(LOG_ERR, "Socket listen Error ");
		exit(-1);
	}
	printf("server: waiting for connections...\n");
	while(1)
	{
		addr_size = sizeof their_addr;
		int new_fd = accept(socket_fd, (struct sockaddr *)&their_addr, &addr_size);

		if (new_fd == -1) {
			close(new_fd);
			syslog(LOG_ERR, "Socket Fail to accept connection ");
			exit(-1);
		}
		else
		{
			inet_ntop(AF_INET, &(their_addr), ip_str, INET_ADDRSTRLEN);
			syslog(LOG_INFO, "Accepted connection from %s", ip_str);
			printf("Accepted connection from %s", ip_str);
		}


		recv_data = recv(new_fd, data_rcv, MAXDATASIZE-1, 0);
		printf("\n Received DAta %s", data_rcv);
		if(recv_data == -1)
		{
			syslog(LOG_ERR, "Socket Fail to receive data: ");
			exit(-1);
		}
		else
		{
			memcpy(data_tx, data_rcv, recv_data);
			printf("\n ------> TX DAta %s", data_tx);
			fd = open(outputfile , O_RDWR | O_CREAT | O_APPEND, 0644 );
			if (fd == -1)
			{
				syslog(LOG_ERR, "Socket Fail to receive data: ");
				close(new_fd);
				continue; 
			}
			else
			{
				printf("\n # Bytes to write: %d", recv_data);
				for(int data_index = 0; data_index < recv_data; data_index ++)
				{
					write(fd, &data_rcv[data_index], 1);
					
					if(write_stat == -1)
					{
						syslog(LOG_ERR, "Writing process fail: ");
						close(fd);
					}
					else
					{
						if(data_rcv[data_index] == 10)
						{
							lseek(fd, 0, SEEK_SET);
							char read_buf[MAXDATASIZE];
							ssize_t bytes_read;
							while ((bytes_read = read(fd, read_buf, MAXDATASIZE)) > 0) {
								send(new_fd, read_buf, bytes_read, 0);
							}
							lseek(fd, 0, SEEK_END);
							
						}
					}
				}
			}

		}
		close(fd);
		close(new_fd);
		syslog(LOG_INFO, "Closed connection from %s", ip_str);
	}
	return 0;
}
