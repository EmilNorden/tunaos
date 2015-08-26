#include "linked_list.h"
#include "definitions.h"
#include "../kernel/memory.h"
#include "../kernel/system.h"
#include "../kernel/util.h"
#include "../drivers/screen.h"
#include <stdint.h>

struct link {
	void *user_data;
	struct link *next;
	struct link *prev;
};

static struct link* list_get_link(struct linked_list *list, size_t index);

int new_linked_list(struct linked_list *list)
{
	if(list == NULL)
		return -1;
		
	list->head = NULL;
	list->tail = NULL;
	return 0;	
}

int list_add_first(struct linked_list *list, void *data)
{
	if(list == NULL)
		return -1;
		
	struct link *new_link = malloc(sizeof(struct link));
	
	if(new_link == NULL)
		return -1;
		
	new_link->user_data = data;
	new_link->next = list->head;
	new_link->prev = NULL;
	list->head = new_link;
	
	return 0;
}

int list_add_last(struct linked_list *list, void *data)
{
	if(list == NULL)
		return -1;
		
	struct link *new_link = malloc(sizeof(struct link));
	
	if(new_link == NULL)
		return -2;
		
	new_link->user_data = data;
	new_link->next = NULL;
	new_link->prev = list->tail;
	
	if(list->tail == NULL) {
		list->head = list->tail = new_link;
	}
	else {
		list->tail->next = new_link;
		list->tail = new_link;
	}
		
	return 0;
}

void *list_at(struct linked_list *list, size_t i)
{
	struct link *l = list_get_link(list, i);
	
	if(l != NULL)
		return l->user_data;
		
	return NULL;
}

int list_remove_at(struct linked_list *list, size_t i)
 {
	struct link *l = list_get_link(list, i); 
	
	if(l->prev)
		l->prev->next = l->next;
		
	if(l->next)
		l->next->prev = l->prev;
		
	if(list->head == l)
		list->head = l->next;
		
	if(list->tail == l)
		list->tail = l->prev;
		
	free(l);
 }
 
 static struct link* list_get_link(struct linked_list *list, size_t i)
 {
	struct link *current = list->head;
	size_t index = 0;
	while(index < i && current) {
		current = current->next;
		index++;
	}
	
	return current;
 }