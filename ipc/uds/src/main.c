/**
 * @file
 *
 * uds - Unix Domain Socket
 *
 * Copyright (c) 2013 RoyLee, All rights reserved.
 *
 */
#include <stdio.h>
#include <getopt.h>
#include <assert.h>

#include "server.h"
#include "client.h"

/*****************************************************************************/

#define DEFAULT_PATH "/tmp/uds"

/*****************************************************************************/

static char *path = 0;
static int isserver = 0;

/*****************************************************************************/

static const char *title_message = \
"UDS - Unix Domain Socket Sample\n"
"Copyright(C) 2013, RoyLee. All rights reserved.\n\n";

static const char *help_message = \
"Usage: uds <-p|--path=<PATH>>[-s|--server][-h|--help]\n"
"  -p, --path=PATH			File Path(Default:/tmp/uds)\n"
"  -s, --sever				Server mode.\n"
"  -h, --help				This help message.\n";

/*****************************************************************************/

static void help(void)
{
	fprintf(stderr, help_message);
}

/**
 * @brief 커맨드 라인 옵션을 파싱한다.
 * @param argc 파라미터 카운트
 * @param argv 파라미터 스트링 배열
 * @return 성공시 0, 실패시 exit() 호출
 */
static int parse_option(int argc, char **argv)
{
	int c, optidx = 0;
	static struct option options[] = {
		{ "path", 1, 0, 'p' },
		{ "server", 0, 0, 's' },
		{ "help", 0, 0, 'h' },
		{ 0, 0, 0, 0 },
	};

	for( ; ; ) {
		c = getopt_long(argc, argv, "p:sh", options, &optidx);
		if(c == -1)
			break;
		switch(c) {
		case 'p':
			if(path) {
				fprintf(stderr, "Path is already setup.(%s)\n", path);
				exit(-1);
			}
			path = strdup(optarg);
			assert(path);
			break;
		case 's':
			isserver = 1;
			break;
		case 'h':
			help();
			exit(0);
			break;
		default:
			return -1;
		}
	}

	if(!path) {
		path = strdup(DEFAULT_PATH);
	}

	return 0;
}

int main(int argc, char **argv)
{
	int ret;

	printf(title_message);
	parse_option(argc, argv);

	if(isserver) {
			start_server(path);
	}
	else {
			start_client(path);
	}

	return ret;
}
