
#ifndef __DLIST_H__
#define __DLIST_H__

#ifdef __DLIST_DEPLOY__
#include "debug.h"
#include "tr.h"
#endif

#include <pthread.h>

typedef struct dllist_node {
void * data;
struct dllist_node *next;
struct dllist_node *prev;
}DLLIST_NODE;

typedef struct dllist{
DLLIST_NODE *head;
DLLIST_NODE *tail;

int (*data_cmp)(void *,void *);

void (*append)(struct dllist*,DLLIST_NODE*);
void (*insert)(struct dllist*,DLLIST_NODE*, DLLIST_NODE*);
void (*remove)(struct dllist*,DLLIST_NODE*);
    void (*remove_at)(struct dllist*, void *);
DLLIST_NODE* (*search)(struct dllist*,void *);

    pthread_mutex_t mutex;
}DLLIST;

/**
 * new_node: Create a new dlist node
 * if size is 0 them link data otherwise copy
 */
DLLIST_NODE * new_node(void *data,int size);
DLLIST *new_dllist( void );
void free_dllist( DLLIST *dll);

void dsetlock_dllist(DLLIST *dll, int status, const char *ff, int line);

#define setlock_dllist(x,y) dsetlock_dllist(x,y,__FUNCTION__,__LINE__)

#ifdef __DLLIST_DEBUG__
#define DEBUG_DLLIST_HEADER   "DEBUG DLLIST:\n\t"
#define DEBUG_DLLIST_NOTIFICATION(arg,...) printf(\
                                           TR_SEPARATOR"\n"DEBUG_DLLIST_HEADER\
                                           arg"\n"TR_SEPARATOR"\n",\
                                           __VA_ARGS__)
#ifdef __DLLOCK_DEBUG__
#define DEBUG_DLLIST_LOCK(arg,...)  DEBUG_DLLIST_NOTIFICATION(arg,__VA_ARGS__)
#else
#define DEBUG_DLLIST_LOCK(arg,...)
#endif

#else
#define DEBUG_DLLIST_NOTIFICATION(arg,...)
#define DEBUG_DLLIST_LOCK(arg,...)
#endif


#endif
