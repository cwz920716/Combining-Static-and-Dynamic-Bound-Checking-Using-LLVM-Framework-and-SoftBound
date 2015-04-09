#ifndef _MTRACK_H
#define _MTRACK_H

#include <stdio.h>

#define __WEAK_INLINE __attribute__((__weak__,__always_inline__)) 

__WEAK_INLINE
void __track_stack_allocation(void *ptr, uint32_t array, size_t stride, size_t alignment)
{
	printf ("alloca %p [%ld x%ld], align %ld\n", ptr, array, stride, alignment);
}

#endif
