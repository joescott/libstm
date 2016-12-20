
#ifndef __STM_H__
#define __STM_H__

#ifdef __STM_DEPLOY
#include "tr.h"
#include "debug.h"
#endif

#include "dlist.h"

#ifdef __STM_DEBUG__
typedef enum{DNONE,DST_LV}STM_DEBUG_LEVEL;
#endif

typedef char* STM_EVENT_ID;
typedef char* STM_STATE_ID;
typedef char* STM_MACHINE_ID;

/**
 * Events
 */
typedef struct {
	STM_EVENT_ID id;
	void *arg;
}STM_EVENT;
STM_EVENT  *newStmEvent( STM_EVENT_ID id, void *arg);

typedef enum {
   N_ST,       /*Normal State*/
   H_ST,       /*History State: remember only current state*/
   HDEEP_ST    /*Deep History State:remember all sub states*/ 
} STM_STATE_TYPE;
/**
 * States
 */
typedef struct stm_state{
	STM_STATE_ID id;
	STM_STATE_TYPE type;

	DLLIST *transition;
#ifdef __ENCAPSULATED__
	void (*addTransition)(struct stm_state *st,void *t);
	void (*removeTransition)(struct stm_state *st,void *t);	
#endif

	DLLIST *subst;
	struct stm_state *father_state;
	struct stm_state *start_substate;
	struct stm_state *history_substate;
	struct stm_state *current_substate;

#ifdef __ENCAPSULATED__
	void (*initSubState)(struct stm_state *st,STM_STATE_ID);
	void (*addSubState)(struct stm_state *st,void *s);	
	void (*removeSubState)(struct stm_state *st,void *s);	
#endif
}STM_STATE;

void addTransition(struct stm_state *st,void *t);
void removeTransition(struct stm_state *st,void *t);	
void initSubState(struct stm_state *st,STM_STATE_ID);
void addSubState(struct stm_state *st,void *s);	
void removeSubState(struct stm_state *st,void *s);	

/**
 * State Machine
 */
typedef struct stm_machine{
	STM_MACHINE_ID id;

	STM_STATE *start_state;
	STM_STATE *current_state;

#ifdef __ENCAPSULATED__
	void (*init)(struct stm_machine *,STM_STATE_ID);
	void (*dispatch)(struct stm_machine *,STM_EVENT *);
#endif

	DLLIST *state;
    DLLIST *event_queue;

#ifdef __ENCAPSULATED__
	void (*addState)(struct stm_machine *stm,void *s);	
	void (*removeState)(struct stm_machine *stm,void *s);	
#endif

#ifdef __STM_DEBUG__
	DEBUG_ST *dbg;
#endif

    int   on;
    void *data;
}STM_MACHINE;

/**
 * Transitions
 */
typedef void (*T_ACTION )(STM_MACHINE *, STM_EVENT *);

typedef struct {
    STM_EVENT_ID event_id;
    STM_STATE *next;
    T_ACTION  action;
} STM_TRANSITION;


void init(struct stm_machine *,STM_STATE_ID);
int  dispatch(struct stm_machine *,STM_EVENT *);
void addState(struct stm_machine *stm,void *s);	
void addToQueue(struct stm_machine *stm,STM_EVENT *);
void removeState(struct stm_machine *stm,void *s);	

STM_TRANSITION * newStmTransition(STM_EVENT_ID id, 
				STM_STATE *next, T_ACTION action);

STM_STATE * newStmState(STM_STATE_ID id,STM_STATE_TYPE type);
STM_MACHINE * newStmMachine(STM_MACHINE_ID id);
void freeStmMachine(STM_MACHINE *stm);
void freeStmState(STM_STATE *st);
void freeStmTransition(STM_TRANSITION *tr);
void freeStmEvent(STM_EVENT *ev);


#ifdef __STM_DEBUG__
#define DEBUG_STM_HEADER              "DEBUG STM:\n\t"
#define DEBUG_STM_ACTION(x,args,...)  printf(TR_SEPARATOR"\n"DEBUG_STM_HEADER"ACTION: %s\n\tInput Event:%s\n\tData:\n"args"\n"TR_SEPARATOR"\n",\
                                    __FUNCTION__,\
                                    x->id,\
                                    __VA_ARGS__)
#else
#define DEBUG_STM_ACTION(x,args,...)
#endif

#endif
