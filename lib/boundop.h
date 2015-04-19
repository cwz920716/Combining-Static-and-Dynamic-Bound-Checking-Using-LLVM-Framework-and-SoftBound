#ifndef _BOUNDOP_H
#define _BOUNDOP_H

#include "mtrack.h"

__WEAK_INLINE
void *__max__(void *a, void *b)
{
	return (a < b) ? b : a;
}

__WEAK_INLINE
void *__min__(void *a, void *b)
{
	return (a > b) ? b : a;
}

#endif
