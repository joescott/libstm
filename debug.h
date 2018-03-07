

#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef __DEBUG_DEPLOY__
#include "tr.h"
#endif

typedef struct {
	int level;
}DEBUG_ST;

#define VERBOSE_CHAR 'v'

DEBUG_ST * dbginit(char *v);
void dbgprintf(DEBUG_ST *d,int verbose,const char *fmt, ...);
void dbgend(DEBUG_ST *d);

/**
 *******************************************
 * MACRO DEBUG DEFINITIONS
 *******************************************
 */
#define DEBUG_LINE printf(TR_SEPARATOR"\nDEBUG LINE %s : %s [%d] \n"TR_SEPARATOR"\n",__FILE__,__FUNCTION__,__LINE__)

/**
 * MTRACE Debug
 *
 * export MALLOC_TRACE="mtrace.log"
 */
#undef __MTRACE_DEBUG__
#ifdef __MTRACE_DEBUG__
#define DEBUG_MTRACE(x) (x)?mtrace():muntrace()
#else
#define DEBUG_MTRACE(x)
#endif

/**
 * Thread Debug
 */
#define __THREAD_DEBUG__

/**
 * Double Linked List Debug
 */
#undef __DLLIST_DEBUG__
#undef  __DLLOCK_DEBUG__

/**
 * Server Debug
 */
#define __SERVER_DEBUG__
#undef __SLOCK_DEBUG__

/**
 * Pipe Debug
 */
#undef __PIPE_DEBUG__
#undef __PIPE_MEM_DEBUG__

/**
 * Queue Debug
 */
#undef __QUEUE_DEBUG__

/**
 * Script Debug
 */
#undef __SCRIPT_DEBUG__

/**
 * State Machine Debug
 */
#define __STM_DEBUG__
#define __STM_LEVEL_DEBUG__  "vvvv"

/**
 * Data Base Debug
 */
#undef __DB_DEBUG__

/**
 * Parser Debug
 */
#undef __TPARSER_DEBUG__

/**
 * Http Debug
 */
#undef __HTTP_DEBUG__


#endif
