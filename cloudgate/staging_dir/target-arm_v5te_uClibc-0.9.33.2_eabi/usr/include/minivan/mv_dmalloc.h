#ifndef MV_DMALLOC_H
#define MV_DMALLOC_H

#include <stdint.h>

typedef struct {
	uintptr_t heap_low_p; //low spot in heap
	uintptr_t heap_high_p; //high spot in heap
	unsigned long total_space; //total, include user space, administrative space and overhead
	unsigned long user_space; //user space, allocated and free
	unsigned long current_allocated; //allocated space
	unsigned long current_pnt_np; //number of pointers (mallocs,callocs, ...)
	unsigned long max_allocated; //max allocated space
	unsigned long max_pnt_np; //max number of pointers (mallocs,callocs, ...)
	unsigned long max_one; //maximum space with one call
} mv_dmalloc_stats_t;

void minivan_register_dmalloc_methods(int* mvfd);

#endif
