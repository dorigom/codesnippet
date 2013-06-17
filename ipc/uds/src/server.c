/**
 * @file
 *
 * uds server
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

void *server_thread(void *arg)
{
	int fd;
	int rd_cnt;
	char buf[255];

	fd = *((int*)arg);

	while(1) {
		rd_cnt = read(fd, buf, 255);
		if(rd_cnt < 0) {
			perror("read error");
			break;
		}
		fprintf(stderr, "%04X>>> %s", fd, (char *)buf);

		write(fd, "recv ok", 255);

		if(strncmp(buf, "quit", 4) == 0) {
			break;
		}
	}
	fprintf(stderr, "Disconnected client(%04X)\n", fd);
	
	close(fd);
}

int start_server(const char *path)
{
	int sfd, cfd;
	struct sockaddr_un serveraddr, clientaddr;
	int stat, client_len;

	int th_id;
	pthread_t thread_t;

	if(access(path, F_OK) == 0) {
		unlink(path);
	}

	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sfd < 0) {
		perror("socket error");
		return -1;
	}
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, path);

	/* bind */
	stat = bind(sfd, (struct sockaddr *)&serveraddr, 
					sizeof(serveraddr));
	if(stat < 0) {
		perror("bind error");
		return -1;
	}

	/* listen */
	stat = listen(sfd, 5);
	if(stat < 0) {
		perror("listen error");
		return -1;
	}

	printf("Ready server...\n");
	while(1) {
		/* accept */
		cfd = accept(sfd, (struct sockaddr *)&clientaddr, &client_len);
		if(cfd < 0) {
			perror("accept error");
			continue;
		}
		printf("Connected Client(%s:%04X)\n", serveraddr.sun_path, cfd);

		/* client thread */
		th_id = pthread_create(&thread_t, NULL, server_thread, 
							(void *)&cfd);
		if(th_id != 0) {
			perror("thread create error");
			continue;
		}

		pthread_detach(thread_t);

	}

	close(sfd);

	return 0;
}
