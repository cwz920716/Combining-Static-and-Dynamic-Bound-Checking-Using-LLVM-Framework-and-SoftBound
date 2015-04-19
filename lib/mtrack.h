#ifndef _MTRACK_H
#define _MTRACK_H

#include <stdio.h>
#include <stdint.h>

#define __WEAK_INLINE __attribute__((__weak__,__always_inline__)) 

__WEAK_INLINE
void __track_stack_allocation_integer_after(void *ptr, size_t array, size_t stride, size_t alignment)
{
	printf ("alloca %p [%ld x i%ld], align %ld\n", ptr, array, stride, alignment);
} 

__WEAK_INLINE
void __track_stack_allocation_bound_after(void *ptr, void *base, void *bound)
{
	printf ("alloca %p ptr_base = %p ptr_bound = %p\n", ptr, base, bound);
} 

__WEAK_INLINE
void __track_heap_allocation_after(void *ptr, size_t size)
{
	printf ("call @malloc %p [%ld x i8]\n", ptr, size);
} 

__WEAK_INLINE
void __track_heap_allocation_bound_after(void *ptr, void *base, void *bound)
{
	printf ("call @malloc %p ptr_base = %p ptr_bound = %p\n", ptr, base, bound);
} 

__WEAK_INLINE
void __track_heap_free_before(void *ptr)
{
	printf ("call @free %p\n", ptr);
} 

__WEAK_INLINE
void __track_stack_save_after(void *ptr)
{
	printf ("call @llvm.stacksave %p\n", ptr);
}  

__WEAK_INLINE
void __track_stack_restore_before(void *ptr)
{
	printf ("call @llvm.stackrestore %p\n", ptr);
} 

__WEAK_INLINE
void __track_load_integer_after(void *ptr, size_t stride, size_t alignment, size_t value)
{
	printf ("load %p [i%ld], align %ld => %ld\n", ptr, stride, alignment, value);
} 

__WEAK_INLINE
void __track_store_integer_before(void *ptr, size_t stride, size_t alignment, size_t value)
{
	printf ("store %p [i%ld], align %ld <= %ld\n", ptr, stride, alignment, value);
}

#endif
