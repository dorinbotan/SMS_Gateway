
#ifndef __SZIP_H__
#define __SZIP_H__

#define _GNU_SOURCE

#include <fcntl.h>
#include <stdint.h>

typedef struct szip_info_t szip_info_t;

typedef ssize_t (*szip_write_func)(void *arg, const void *buf, size_t count);



szip_info_t *szip_create_info(const char *files[], unsigned int numfiles, const char * base, const char * zbase);

void szip_free_info(szip_info_t * info);

u_int64_t szip_calc_size(szip_info_t * info, int cdir);

int szip_stream_zip_fd(int fd, szip_info_t *info, unsigned int chunk_size);

int szip_stream_zip(szip_info_t * info, szip_write_func szip_write, void * warg, unsigned int chunk_size);



#endif

