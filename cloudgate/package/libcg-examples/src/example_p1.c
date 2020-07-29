#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>

#define FILE_DEV "/dev/p1-0"

#define MAX_FRAME_SIZE 1068

static void print_frame(const char *frame_buffer)
{
	printf("--BEGIN------------------------------------------------------------------\n");
	if ( !frame_buffer || (strlen(frame_buffer)>MAX_FRAME_SIZE))
		return;
	printf(frame_buffer);
	printf("--END--------------------------------------------------------------------\n");
}

int main()
{
	int fd;
	int ret;
	struct pollfd pfd = {0};
	char frame[MAX_FRAME_SIZE+1] = {0};

	printf("DSRM P1 logging example\n");
	printf("opening file %s\n",FILE_DEV);
	if((fd = open(FILE_DEV, O_RDONLY)) < 0)
	{
		perror("open()");
		exit(-1);
	}

	// this step is not required!
	//the P1 data is received every 10 seconds the previous buffer can still be retrieved
	printf("Reading last received buffered data ...\n");
	ret = read(fd, frame, MAX_FRAME_SIZE);
	if(ret < 0) {
		perror("read()");
		exit(-1);
	}
	else {
		print_frame(frame);
	}

	// wait for the next frame and print it
	pfd.fd = fd;
	pfd.events = POLLIN;
	while(1) {
		ret = poll(&pfd, 1, 15000);
		if(ret < 0)
		{
			perror("poll()");
			exit(-1);
		}
		if( ret >0) {
			memset(frame, 0x0, sizeof(frame));
			ret = read(fd, frame, MAX_FRAME_SIZE);
			if(ret < 0)
			{
				printf("Error reading value file\n");
				continue;
			}
			print_frame(frame);
		}
	}
	return 0;
}

