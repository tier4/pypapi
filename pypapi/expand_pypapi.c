/****************************/
/* THIS IS OPEN SOURCE CODE */
/****************************/

/** 
* @file:    pypapi_to_papi.c
*
* @brief Expand low-level API is here.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include "../papi/src/papi.h"
#include "papi_internal.h"
#include "papi_vector.h"
#include "papi_memory.h"
#include "papi_preset.h"

#include "cpus.h"
#include "extras.h"
#include "sw_multiplex.h"


/*******************************/
/* BEGIN EXTERNAL DECLARATIONS */
/*******************************/

#define DEBUG

extern hwi_presets_t user_defined_events[PAPI_MAX_USER_EVENTS];
extern int user_defined_events_count;


#ifdef DEBUG
#define papi_return(a) do { \
	int b = a; \
	if (b != PAPI_OK) {\
		_papi_hwi_errno = b;\
	} \
	APIDBG("EXIT: return: %d\n", b);\
	return((_papi_hwi_debug_handler ? _papi_hwi_debug_handler(b) : b)); \
} while (0)
#else
#define papi_return(a) do { \
	int b = a; \
	if (b != PAPI_OK) {\
		_papi_hwi_errno = b;\
	} \
	APIDBG("EXIT: return: %d\n", b);\
	return(b);\
} while(0)
#endif


/*
#ifdef DEBUG
#define papi_return(a) return((_papi_hwi_debug_handler ? _papi_hwi_debug_handler(a) : a))
#else
#define papi_return(a) return(a)
#endif
*/

#ifdef DEBUG
extern int _papi_hwi_debug;
#endif


////static int init_retval = DEADBEEF;

inline_static int
valid_component( int cidx )
{
	if ( _papi_hwi_invalid_cmp( cidx ) )
		return ( PAPI_ENOCMP );
	return ( cidx );
}

inline_static int
valid_ESI_component( EventSetInfo_t * ESI )
{
	return ( valid_component( ESI->CmpIdx ) );
}

/**************************************************************/
/**************************************************************/
/**************************************************************/
int
PAPI_set_inherit( int EventSet )
{
	APIDBG( "!!! Entry: EventSet: %d to inherit\n", EventSet);

	PAPI_option_t inh;
	EventSetInfo_t *ESI;
	int cidx;

	/* Is the EventSet already in existence? */

	ESI = _papi_hwi_lookup_EventSet( EventSet );

	if ( ESI == NULL )
		papi_return( PAPI_ENOEVST );

	/* if the eventset has no index return NOCMP */
	cidx = valid_ESI_component( ESI );
	if ( cidx < 0 )
		papi_return( cidx );

	//if ( ( ret = mpx_check( EventSet ) ) != PAPI_OK )
	//	papi_return( ret );

	memset( &inh, 0x0, sizeof ( inh ) );
	inh.inherit.eventset = EventSet;
	inh.inherit.inherit = PAPI_INHERIT_ALL;
	return ( PAPI_set_opt( PAPI_INHERIT, &inh ) );
}

int
PAPI_set_domain_to_eventset( int domain, int EventSet )
{
	APIDBG( "!!! Entry: EventSet: %d to domain %d\n", EventSet, domain);

	PAPI_domain_option_t domain_opt;
	EventSetInfo_t *ESI;
	int cidx;

	/* Is the EventSet already in existence? */

	ESI = _papi_hwi_lookup_EventSet( EventSet );

	if ( ESI == NULL )
		papi_return( PAPI_ENOEVST );

	/* if the eventset has no index return NOCMP */
	cidx = valid_ESI_component( ESI );
	if ( cidx < 0 )
		papi_return( cidx );

	memset( &domain_opt, 0x0, sizeof ( domain_opt ) );
	domain_opt.def_cidx=cidx;
	domain_opt.eventset=EventSet;
	domain_opt.domain=domain;
	return ( PAPI_set_opt( PAPI_DOMAIN, (PAPI_option_t*)&domain_opt ) );
}

int
PAPI_set_granurality_to_eventset( int gran, int EventSet )
{
	APIDBG( "!!! Entry: EventSet: %d to %d\n", EventSet, gran);

	PAPI_granularity_option_t gran_opt;
	EventSetInfo_t *ESI;
	int cidx;

	/* Is the EventSet already in existence? */

	ESI = _papi_hwi_lookup_EventSet( EventSet );

	if ( ESI == NULL )
		papi_return( PAPI_ENOEVST );

	/* if the eventset has no index return NOCMP */
	cidx = valid_ESI_component( ESI );
	if ( cidx < 0 )
		papi_return( cidx );

	memset( &gran_opt, 0x0, sizeof ( gran_opt ) );
	gran_opt.def_cidx=cidx;
	gran_opt.eventset=EventSet;
	gran_opt.granularity=gran;
	return ( PAPI_set_opt( PAPI_GRANUL, (PAPI_option_t*)&gran_opt ) );
}

int
PAPI_get_event_name_to_code( char *eventName )
{
	int out;
	int rc = PAPI_event_name_to_code( eventName, &out );
	if( rc != PAPI_OK)
	{
		APIDBG( "!!! Entry: ERR Event: %s to %d\n", eventName, rc);
		return -1;
	}
	return out;
}
