
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stm.h"

#ifdef __STM_DEBUG__
static char* debug_type[]={"ST","H","H+"};
#endif


STM_EVENT *
newStmEvent( STM_EVENT_ID id, void *arg)
{
	STM_EVENT *ev = (STM_EVENT *)malloc(sizeof(STM_EVENT));
	ev->id = strdup(id);
	ev->arg = arg;

	return ev;
}

void 
freeStmEvent(STM_EVENT *ev)
{
	free(ev->id);
	free(ev);
}


STM_TRANSITION *
newStmTransition( STM_EVENT_ID id, STM_STATE *next, T_ACTION action)
{
	STM_TRANSITION *tr = (STM_TRANSITION *)malloc(sizeof(STM_TRANSITION));
	tr->event_id = strdup(id);
	tr->next = next;
	tr->action = action;

	return tr;
}

void 
freeStmTransition(STM_TRANSITION *tr)
{
	free(tr->event_id);
}


static int 
transition_cmp(void *d1,void *d2)
{

	STM_TRANSITION *tr1 = (STM_TRANSITION *) d1;
	STM_EVENT_ID id2 = (STM_EVENT_ID) d2;
	return (strcmp(tr1->event_id,id2) == 0);
}

static int 
subst_cmp(void *d1,void *d2)
{
	STM_STATE *st1 = (STM_STATE *) d1;
	STM_STATE_ID id2 = (STM_STATE_ID) d2;
	return (strcmp(st1->id,id2) == 0);
}

void 
addTransition(STM_STATE *st,void *s)
{
	(*st->transition->append)(st->transition,new_node(s,0));
}

void 
removeTransition(STM_STATE *st,void *s)
{
	DLLIST_NODE *lnode;
	if((lnode = (*st->transition->search)(st->transition,s)) != NULL)
		(*st->transition->remove)(st->transition,lnode);
}

void 
addSubState(STM_STATE *st,void *s)
{
	((STM_STATE *)s)->father_state = st;
	(*st->subst->append)(st->subst,new_node(s,0));
}

void 
removeSubState(STM_STATE *st,void *s)
{
	DLLIST_NODE *lnode;
	if((lnode = (*st->subst->search)(st->subst,s)) != NULL)
		(*st->subst->remove)(st->subst,lnode);
}

void
initSubState(STM_STATE *st, STM_STATE_ID id)
{
	DLLIST_NODE *lnode =
		 (*st->subst->search)(st->subst,id);
	if(lnode->data != NULL)
		st->start_substate = st->current_substate = lnode->data;
}


STM_STATE *
newStmState(STM_STATE_ID id, STM_STATE_TYPE type)
{
	STM_STATE *st = (STM_STATE *)malloc(sizeof(STM_STATE));
	st->id = strdup(id);
	st->type = type;
	st->transition = new_dllist();
	st->transition->data_cmp = transition_cmp;
    st->current_substate = NULL;
#ifdef __ENCAPSULATED__
	st->addTransition = addTransition;
	st->removeTransition = removeTransition;
#endif
	st->subst = new_dllist();
	st->subst->data_cmp = subst_cmp;
#ifdef __ENCAPSULATED__
	st->initSubState = initSubState;
	st->addSubState = addSubState;
	st->removeSubState = removeSubState;
#endif
	
	return st;
}

void 
freeStmState(STM_STATE *st)
{
	DLLIST_NODE *lnode;
	free(st->id);

	for(lnode = st->transition->head; lnode != NULL; lnode = lnode->next) 
		freeStmTransition((STM_TRANSITION *) lnode->data);
	free_dllist(st->transition);

	for(lnode = st->subst->head; lnode != NULL; lnode = lnode->next) 
		freeStmState((STM_STATE *) lnode->data);
	free_dllist(st->subst);
}

static void
reinitState(STM_STATE *st)
{
	STM_STATE *stl = st->current_substate;
	st->history_substate = st->current_substate;
	st->current_substate = st->start_substate;
	if(stl)
		reinitState(stl);
}

static void
deepHist(STM_STATE *st)
{
	STM_STATE *stl = st->current_substate;
	st->current_substate = st->history_substate;
	if(stl)
		deepHist(stl);
}

static int 
isEventTransition(STM_MACHINE *stm,STM_STATE **state,STM_EVENT *event)
{
	DLLIST_NODE  *tran_node;
	STM_STATE *st = *state;
#ifdef __STM_DEBUG__
    dbgprintf(stm->dbg,DST_LV,DEBUG_STM_HEADER"%s---(%s)|",st->id,event->id);
#endif
	DLLIST *tl = st->transition;
    if((tran_node = (*tl->search)(tl,event->id))!=NULL) {
		STM_TRANSITION *tran = tran_node->data;
#ifdef __STM_DEBUG__
		dbgprintf(stm->dbg,DST_LV,"%s|",debug_type[tran->next->type]);
#endif
        switch(tran->next->type) {
			case H_ST:
				*state = tran->next->father_state; 
				(*state)->current_substate = (*state)->history_substate;
				break;
			case HDEEP_ST:
				*state = tran->next->father_state; 
				(*state)->current_substate = (*state)->history_substate;
				deepHist((*state));
				break;
			default:	
				*state = tran->next;
		}

		if(st->current_substate)
			reinitState(st);
#ifdef __STM_DEBUG__
		dbgprintf(stm->dbg,DST_LV,"--->%s\n",(*state)->id);
#endif
		if(tran->action)
            (*tran->action)(stm, event);

		return 1;
    } else if(st->current_substate) {
		return isEventTransition(stm,&st->current_substate,event);
	}
	return 0;
} 

int
dispatch(STM_MACHINE *stm,STM_EVENT *event)
{
    int ev_ack = -1;
    if(event == NULL && stm->event_queue->head != NULL) {
        STM_EVENT *ev = (STM_EVENT *)stm->event_queue->head->data;
        event = newStmEvent(ev->id,ev->arg);
        free(ev->id);
        (*stm->event_queue->remove)
        (stm->event_queue,stm->event_queue->head);
    }
    if(event != NULL) {
        ev_ack = isEventTransition(stm,&stm->current_state,event);
	freeStmEvent(event);
    }

#ifdef __STM_DEBUG__
	dbgprintf(stm->dbg,DST_LV,"%s\n",ev_ack?"":"X");
#endif
    return ev_ack;
}

void
addToQueue(struct stm_machine *stm,STM_EVENT *event)
{
    (*stm->event_queue->append)
    (stm->event_queue,new_node((void *)event,0));
}

void
init(STM_MACHINE *stm, STM_STATE_ID id)
{
	DLLIST_NODE *lnode =
		 (*stm->state->search)(stm->state,id);
	if(lnode->data != NULL)
		stm->start_state = stm->current_state = lnode->data;

    stm->on= 1;
}

static int 
state_cmp(void *d1,void *d2)
{
	STM_STATE *st1 = (STM_STATE *) d1;
	STM_STATE_ID id2 = (STM_STATE_ID) d2;
	return (strcmp(st1->id,id2) == 0);
}

static int
event_cmp(void *d1,void *d2)
{
    STM_EVENT *evt1 = (STM_EVENT *) d1;
    STM_EVENT_ID id2 = (STM_EVENT_ID) d2;
    return (strcmp(evt1->id,id2) == 0);
}

void 
addState(STM_MACHINE *stm,void *s)
{
	(*stm->state->append)(stm->state,new_node(s,0));
}

void 
removeState(STM_MACHINE *stm, void *s)
{
	DLLIST_NODE *lnode;
	if((lnode = (*stm->state->search)(stm->state,s)) != NULL)
		(*stm->state->remove)(stm->state,lnode);
}

STM_MACHINE *
newStmMachine(STM_MACHINE_ID id)
{
	STM_MACHINE *stm = (STM_MACHINE *)malloc(sizeof(STM_MACHINE));
	stm->id = strdup(id);

	stm->state = new_dllist();
	stm->state->data_cmp = state_cmp;

    stm->event_queue = new_dllist();
    stm->event_queue->data_cmp = event_cmp;

#ifdef __ENCAPSULATED__
	stm->dispatch = dispatch;
	stm->init = init;
	stm->addState = addState;
	stm->removeState = removeState;
#endif

#ifdef __STM_DEBUG__
    stm->dbg = dbginit(__STM_LEVEL_DEBUG__);
#endif

	return stm; 
}

void 
freeStmMachine(STM_MACHINE *stm)
{
	DLLIST_NODE *lnode;
	free(stm->id);

	for(lnode = stm->state->head; lnode != NULL; lnode = lnode->next) 
		freeStmState((STM_STATE *) lnode->data);
	free_dllist(stm->state);

    for(lnode = stm->event_queue->head; lnode != NULL; lnode = lnode->next)
        freeStmEvent((STM_EVENT *) lnode->data);
    free_dllist(stm->event_queue);
	
#ifdef __STM_DEBUG__
	dbgend(stm->dbg);
#endif
	free(stm);
}
