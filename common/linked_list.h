#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stddef.h>

struct linked_list {
	struct link *head;
	struct link *tail;
};

struct link;

int new_linked_list(struct linked_list *list);
int list_add_first(struct linked_list *list, void *data);
int list_add_last(struct linked_list *list, void *data);
void *list_at(struct linked_list *list, size_t i);
int list_remove_at(struct linked_list *list, size_t i);


#endif