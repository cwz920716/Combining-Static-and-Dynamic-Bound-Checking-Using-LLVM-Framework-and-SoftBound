#include <stdio.h>
#include <stdlib.h>
#include "../lib/mtrack.h"

#define T int

struct Node;
typedef struct Node *node_ptr;
typedef struct Node node_t;

struct Node {
	T item;
	node_ptr next;
};

node_t head;

int hasNext(node_ptr cur) {
	return cur->next != NULL;
}

node_ptr next(node_ptr cur) {
	return cur->next;
}

struct Node copy(node_ptr cur) {
	return (*cur);
}

void push(node_ptr cur, node_ptr n) {
	n->next = cur->next;
	cur->next = n;
}

node_ptr newNode(T item) {
	node_ptr n = (node_ptr) malloc(sizeof(node_t));
	n->item = item;
	return n; 
}

node_ptr removeFrom(node_ptr n) {
	if (!hasNext(n))
		return NULL;

	node_ptr ret = n->next;
	n->next = ret->next;
	return ret;
}

void freeNode(node_ptr n) {
	free(n);
}
	
int main() {
	int i = 0; 
	node_ptr head_ptr = &head;

	for (i = 0; i < 10; i++) {
		node_ptr n = newNode(i);
		push(head_ptr, n);
	}

	while (hasNext(head_ptr)) {
		node_ptr np = removeFrom(head_ptr);
		struct Node n = copy(np);
		printf("--%d--\n", n.item);
		freeNode(np);
	}

	return 0;
}
