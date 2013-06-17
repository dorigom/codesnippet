/**
 * @file
 *
 * uds client
 *
 * Copyright (c) 2013 RoyLee, All rights reserved.
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>

int start_client(const char *path)
{
	int sfd, cfd;
	struct sockaddr_un serveraddr, clientaddr;
	int stat, client_len;

	char buf_in[255];
	char buf_get[255];

	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sfd < 0) {
		perror("socket error");
		return -1;
	}
	bzero(&clientaddr, sizeof(clientaddr));
	clientaddr.sun_family = AF_UNIX;
	strcpy(clientaddr.sun_path, path);
	client_len = sizeof(clientaddr);

	/* connect */
	if(connect(sfd, (struct sockaddr *)&clientaddr, client_len) < 0) {
		perror("connect error");
		return -1;
	}

	printf("Connected to server...\n");
	while(1) {
		printf("Message: ");
		fgets(buf_in, 255, stdin);
		printf("<<< %s", buf_in);

		buf_in[strlen(buf_in) - 1] = '\n';
	
		write(sfd, buf_in, 255);
		while(1) {
			read(sfd, buf_get, 255);
			printf(">>> %s\n", buf_get);
			break;
		}
		if(strncmp(buf_in, "quit", 4) == 0) {
			break;
		}
	}

	close(sfd);

	return 0;
}
