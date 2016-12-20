#include <stdlib.h>
#include <stdio.h>
#include "dlist.h" 

static int 
int_cmp(void *d1,void *d2)
{
	return (*(int *)d1 == *(int *)d2);
}

int 
main(void) 
{
    int i = 0;
    DLLIST_NODE *lnode;
    DLLIST *dll = new_dllist();
    dll->data_cmp = int_cmp;
    /* add some numbers to the double linked list */
    for(i = 0; i <= 5; i++)
    {
        int *d = malloc(sizeof(int));
        *d = i;
        (*dll->append)(dll,new_node((void *)d,sizeof(int)));
        free(d);
    }
#if 0
    /* print the dll list */
    for(lnode = dll->head; lnode != NULL; lnode = lnode->next) 
        printf("%d\n", *(int *)lnode->data);

    i = 4;
    if((lnode = (*dll->search)(dll,(void *)&i)) != NULL)
        (*dll->remove)(dll,lnode);
    i = 5;
    if((lnode = (*dll->search)(dll,(void *)&i)) != NULL)
    {
        i = 100;
        (*dll->insert)(dll,new_node((void *)&i,sizeof(i)),lnode);
    }
#endif

    /* print the dll list */
    for(lnode = dll->head; lnode != NULL; lnode = lnode->next) {
        printf("%d\n", *(int *)lnode->data);
        (*dll->remove)(dll,lnode);
    }
    /* destroy the dll list */
    free_dllist(dll);
    return 0;
}

