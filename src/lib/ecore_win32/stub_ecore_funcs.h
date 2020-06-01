#ifndef STUB_ECORE_FUNCS_H
#define STUB_ECORE_FUNCS_H

// We shoud try to separete ecore and ecore_input

/*          ECORE FUNCS         */
void* ecore_event_add (void * arg, ...);

void* ecore_event_init (void);

void* ecore_event_type_new (void);

void ecore_event_shutdown (void);

void* ecore_event_type_flush (void * arg, ...);

/*          ECORE_INPUT FUNCS         */
#endif
