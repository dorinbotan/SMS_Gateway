
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FILE_DEV "/dev/p1-0"
#define BUFFER_SIZE 4096
/*
 * This program demostrates the option O_NONBLOCK
 * When the option O_NONBLOCK is given on the open call, read will return immediately,
 * even when no data is present.
 * The program shows how to retrieve the last p1 frame without waiting for interrupt and
 * shows also how you can retrieve a complete frame based by using a single read or a character based read.
 */

int main(int argc, char *argv[])
{

	fd_set rdset, wrset;
	int  n;
	int tty_fd;
	int last=0,nolast=0;
	int total_chars_read=0;
	int block_read = 0;
	int char_based_read = 0;
	char buffer[BUFFER_SIZE];

	if(argc<3) {
		printf("example_p1 demostrates not blocking reads form the /dev/p1-0 p1 interface\n");
		printf("example_p1_nonblock <last|nolast> <block|char>\n");
		printf("parameter last:   application reads last frame, then it uses select to retrieve next p1 frames\n");
		printf("parameter nolast: application doesn't read last frame and calls before first read select\n");
		printf("parameter block:  application reads p1 frame by one single read\n");
		printf("parameter char:   application reads p1 frame by multiple character based reads\n");
		exit(-1);
	} else {
		if (strcmp(argv[1],"last")==0) last = 1;
		if (strcmp(argv[1],"nolast")==0) nolast = 1;
		if (strcmp(argv[2],"block")==0) block_read = 1;
		if (strcmp(argv[2],"char")==0) char_based_read = 1;
		if((last ==0 && nolast ==0)||(block_read ==0 && char_based_read ==0)) {
			printf("not all required parametes found\n");
			exit(-1);
		}
	}

	tty_fd = open(FILE_DEV, O_RDONLY | O_NONBLOCK);
	if (tty_fd < 0) {
		printf("cannot open %s: %s", FILE_DEV, strerror(errno));
		exit(-1);
	}

	if(last) {
	// read last p1 frame, without waiting interrupts
		int i;

		unsigned char c;
		// we need at least 2 reads to fetch the last p1 frame
		// i=0: first read needed to schedule a worker to fetch the p1 frame into the driver
                // i=1: actually read the frame
		for(i=0;i<2;i++) {
			if(block_read==0) {
				do {
					n = read(tty_fd, &c, 1);
					if(n==1) {
						total_chars_read++;
						printf("%c",c);
					}
				} while (n == 1);
			} else {
				// read the complete frame ( BUFFER_SIZE > max frame size = 1078 )
				n = read(tty_fd, buffer, BUFFER_SIZE);
				if (n>0 && n<BUFFER_SIZE) {
					buffer[n]=0;
					total_chars_read+=n;
					printf("%s",buffer);
				}
			}
			if (n == 0) {
				printf("port closed");
				break;
			}
			else if ( n == - EPERM ) {
				if ( errno == EAGAIN) {
					// This error tells us the resource is temporary unavailable, and the errno
					// suggests us to retry the read
					if(i==0) {
						// This is the first read: we used the read to retrieve the p1 frame from the cortex
						// if a frame is present, the next will retrieve the data, so do extra read cycles to retrieve the
						// complete p1 frame
						// but sleep a while, to give the driver time to retrieve the p1 frame
						sleep(1);
						continue;
					} else {
						// frame should be completely read, if it was present
						// time to read for new frame
						break;
					}
				}
			}
			else if ( n < 0  ) {
				printf("read from port failed: %s", strerror(errno));
				break;
			}
		}
	}
	if ((total_chars_read == 0) && last) {
		printf("no last p1 frame was found, please check if meter is connected\n");
	}
	// use interrupts
	for (;;) {
		FD_ZERO(&rdset);
		FD_ZERO(&wrset);
		FD_SET(tty_fd, &rdset);

		if (select(tty_fd+1, &rdset, &wrset, NULL, NULL) < 0) {
			printf("select failed: %d : %s", errno, strerror(errno));
			break;
		}
		if ( FD_ISSET(tty_fd, &rdset) ) {

			unsigned char c;
			/* read from port 
			 * remark: if you don't read all available data and call select
			 * select will return immediately, even if no interrupt received.
			 * To be sure that select blocks until arrival next p1 frame you must
			 * read the complete p1 frame before calling select
			 */ 
			if(block_read==0) {
				do {
					n = read(tty_fd, &c, 1);
					if(n==1) printf("%c",c);
				} while (n == 1);
			} else {
				n = read(tty_fd, buffer, BUFFER_SIZE);
				if (n>0 && n<BUFFER_SIZE) {
					buffer[n]=0;
					printf("%s",buffer);
				}
			}

			if (n == 0) {
				printf("port closed");
				break;
			}
			else if ( n == - EPERM ) {
				if ( errno == EAGAIN) {
					// complete frame has been read, let select wait for next p1 frame
					continue;
				}
			}
			else if ( n < 0  ) {
				printf("read from port failed: %s", strerror(errno));
				break;
			}
		}
	}
	close(tty_fd);
}

