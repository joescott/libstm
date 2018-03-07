
#include "dlist.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static void 
append_node(DLLIST *dll,DLLIST_NODE *lnode) 
{
    setlock_dllist(dll,1);
    if(dll->head == NULL) {
		dll->head = lnode;
		lnode->prev = NULL;
	} else {
		dll->tail->next = lnode;
		lnode->prev = dll->tail;
	}

	dll->tail = lnode;
	lnode->next = NULL;
    setlock_dllist(dll,0);
}

static void 
insert_node(DLLIST *dll,DLLIST_NODE *lnode, DLLIST_NODE *after) 
{
    setlock_dllist(dll,1);
	lnode->next = after->next;
	lnode->prev = after;

	if(after->next != NULL)
		after->next->prev = lnode;
	else
		dll->tail = lnode;

	after->next = lnode;
    setlock_dllist(dll,0);
}

static void 
remove_node(DLLIST *dll,DLLIST_NODE *lnode) 
{
    setlock_dllist(dll,1);
	if(lnode->prev == NULL)
		dll->head = lnode->next;
	else
		lnode->prev->next = lnode->next;

	if(lnode->next == NULL)
		dll->tail = lnode->prev;
	else
		lnode->next->prev = lnode->prev;
	
	free(lnode->data);
	free(lnode);
    setlock_dllist(dll,0);
}

static void
removeat_node(DLLIST *dll,void *data)
{
    DLLIST_NODE *lnode;

    if(dll->data_cmp == NULL)
        return; /*CMP Function not defined*/

    setlock_dllist(dll,1);
    for(lnode = dll->head; lnode != NULL;
            lnode = lnode->next)
        if((*dll->data_cmp)(lnode->data,data)) {
            if(lnode->prev == NULL)
                dll->head = lnode->next;
            else
                lnode->prev->next = lnode->next;

            if(lnode->next == NULL)
                dll->tail = lnode->prev;
            else
                lnode->next->prev = lnode->prev;

            free(lnode->data);
            free(lnode);

            setlock_dllist(dll,0);
            break;
        }
    setlock_dllist(dll,0);
}

static DLLIST_NODE * 
search_node(DLLIST *dll,void *data) 
{
	DLLIST_NODE *lnode;

	if(dll->data_cmp == NULL) 
		return NULL; /*CMP Function not defined*/

    setlock_dllist(dll,1);
	for(lnode = dll->head; lnode != NULL;
			lnode = lnode->next)
        if((*dll->data_cmp)(lnode->data,data)) {
            setlock_dllist(dll,0);
			return lnode;
        }
    setlock_dllist(dll,0);
	return NULL;
}


DLLIST_NODE * 
new_node(void *data, int size )
{
	DLLIST_NODE * lnode = (DLLIST_NODE *)malloc(sizeof(DLLIST_NODE));
    if(size) {
		lnode->data = (void *)malloc(size);
		memcpy(lnode->data,data,size);
	}else
		lnode->data = data;

	return lnode; 
}

DLLIST *
new_dllist( void )
{
	DLLIST *dll = (DLLIST *) malloc(sizeof(DLLIST));
	dll->head = NULL;
	dll->tail = NULL;
	dll->data_cmp = NULL;

	dll->append = append_node;
	dll->insert = insert_node;
	dll->remove = remove_node;
    dll->remove_at = removeat_node;
	dll->search = search_node;

    pthread_mutex_init(&dll->mutex,NULL);
	return dll;
}

void 
free_dllist( DLLIST *dll)
{
	while(dll->head != NULL)
		(*dll->remove)(dll,dll->head);

    pthread_mutex_destroy(&dll->mutex);
	free(dll);
}

void
dsetlock_dllist(DLLIST *dll, int status, const char *ff, int line)
{
    DEBUG_DLLIST_LOCK("%s[%d] DLL Set ->LOCK[%d] [%ld]",
                      ff,line,
                      status,pthread_self());

    if(status)
        pthread_mutex_lock(&(dll->mutex));
    else
        pthread_mutex_unlock(&(dll->mutex));

    DEBUG_DLLIST_LOCK("%s[%d] DLL Set <-LOCK[%d] [%ld]",
                      ff,line,
                      status,pthread_self());
}

