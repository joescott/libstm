#include <stdlib.h>
#include <stdio.h>
#include "stm.h" 


static void
test(STM_MACHINE *stm, STM_EVENT *event)
{
	printf("\tACTION: %s\n",__FUNCTION__);
	printf("\tInput Event:%s (%s)\n",event->id,(char *)event->arg);
}

int 
main(void) 
{
	STM_MACHINE *	stm = newStmMachine("test");	
	/** 
	 * States Definitions
	 */
	STM_STATE * 	st1 = newStmState("ST1",N_ST);

	STM_STATE * 	st11 = newStmState("ST11",N_ST);
	STM_STATE *	h13 = newStmState("H13",H_ST);
	STM_STATE *	h14 = newStmState("H14",HDEEP_ST);

	STM_STATE *	st12 = newStmState("ST12",N_ST);
	STM_STATE * 	st121 = newStmState("ST121",N_ST);
	STM_STATE *	st122 = newStmState("ST122",N_ST);

	STM_STATE *	st2 = newStmState("ST2",N_ST);

	/** 
	 * Transitions
	 */
	STM_TRANSITION *tr1 = newStmTransition("EV1",st2,test);
	STM_TRANSITION *tr2 = newStmTransition("EV2",st1,test);
	STM_TRANSITION *tr3 = newStmTransition("EV3",h13,test);
	STM_TRANSITION *tr4 = newStmTransition("EV4",h14,test);

	STM_TRANSITION *tr11 = newStmTransition("EV11",st12,test);
	STM_TRANSITION *tr12 = newStmTransition("EV12",st11,test);

	STM_TRANSITION *tr121 = newStmTransition("EV121",st122,test);
	STM_TRANSITION *tr122 = newStmTransition("EV122",st121,test);

#if 1
	addTransition(st1, tr1);
	addTransition(st2, tr2);
	addTransition(st2, tr3);
	addTransition(st2, tr4);

	addTransition(st11, tr11);
	addTransition(st12, tr12);

	addTransition(st121, tr121);
	addTransition(st122, tr122);

	addSubState(st1, st11);

	addSubState(st12, st121);
	addSubState(st12, st122);
	addSubState(st1, st12);

	addSubState(st1, h13);
	addSubState(st1, h14);

	initSubState(st1, "ST11");
	initSubState(st12, "ST121");

	addState(stm, st1);
	addState(stm, st2);

	init(stm, "ST1");

	dispatch(stm,newStmEvent("EV11","test"));
	dispatch(stm,newStmEvent("EV1","test"));
	dispatch(stm,newStmEvent("EV2","test"));
	dispatch(stm,newStmEvent("EV12","test"));
	dispatch(stm,newStmEvent("EV11","test"));
	dispatch(stm,newStmEvent("EV121","test"));
	dispatch(stm,newStmEvent("EV1","test"));
	dispatch(stm,newStmEvent("EV3","test"));
	dispatch(stm,newStmEvent("EV121","test"));
	dispatch(stm,newStmEvent("EV1","test"));
	dispatch(stm,newStmEvent("EV2","test"));
	dispatch(stm,newStmEvent("EV11","test"));
	dispatch(stm,newStmEvent("EV122","test"));
	dispatch(stm,newStmEvent("EV121","test"));

	//dispatch(stm,newStmEvent("EV4","test"));
#endif


#ifdef __ENCAPSULATED__
	(*st1->addTransition)(st1, tr1);
	(*st2->addTransition)(st2, tr2);

	(*st11->addTransition)(st11, tr11);
	(*st12->addTransition)(st12, tr12);

	(*st1->addSubState)(st1, st11);
	(*st1->addSubState)(st1, st12);
	(*st1->initSubState)(st1, "ST11");

	(*stm->addState)(stm, st1);
	(*stm->addState)(stm, st2);

	(*stm->init)(stm, "ST1");

	(*stm->dispatch)(stm,newStmEvent("EV11","test"));
	(*stm->dispatch)(stm,newStmEvent("EV1","test"));
	(*stm->dispatch)(stm,newStmEvent("EV2","test"));
	(*stm->dispatch)(stm,newStmEvent("EV12","test"));
#endif
	freeStmMachine(stm);

	return EXIT_SUCCESS;
}

