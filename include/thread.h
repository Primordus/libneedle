#pragma once
#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif


/**
 * This module contains wrapper functions around the C11 thread API that use an 
 * opaque pointer iso thrd_t. Provides a few helper functions to give the
 * caller information on how to allocate the struct.
 *
 * API is exactly the same for the rest.
 */


struct thread;
struct timespec;
typedef void (*thread_start_t)(void *arg);
typedef size_t thread_id_t;


/**
 * Helper functions for struct thread:
 */


int thread_new(struct thread **t);
size_t get_thread_data_size(void);
void thread_free(struct thread **t);


/**
 * Wrapper functions around thrd_t API:
 */

int thread_create(struct thread *t, thread_start_t callback, void *arg);
int thread_equal(const struct thread * const t1, const struct thread * const t2);  // non-0 if equal, else 0
int thread_current(thread_id_t *id);
int thread_sleep(const struct timespec *time_point);
void thread_yield(void);
int thread_detach(struct thread *t);
int thread_join(struct thread *t);

#ifdef __cplusplus
}
#endif

