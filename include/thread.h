#pragma once
#include <threads.h>
#include <stdint.h>


/**
 * This module contains wrapper functions around the C11 thread API that use an 
 * opaque pointer iso thrd_t. Provides a few helper functions to give the
 * caller information on how to allocate the struct.
 *
 * API is exactly the same for the rest.
 */

static const int THREAD_NOT_VALID = (int) 0x1000;


struct thread;
struct timespec;
typedef thrd_start_t thread_start_t;


/**
 * Helper functions for struct thread:
 */


int thread_new(struct thread **t);
size_t get_thread_data_size(void);
void thread_free(struct thread *t);


/**
 * Wrapper functions around thrd_t API:
 */

int thread_create(struct thread *t, thread_start_t callback, void *arg);
int thread_equal(const struct thread * const t1, const struct thread * const t2);  // non-0 if equal, else 0
int thread_current(struct thread *t);
int thread_sleep(const struct timespec* time_point, struct timespec *remaining);
void thread_yield(void);
_Noreturn void thread_exit(int result);
int thread_detach(const struct thread *t);
int thread_join(const struct thread *t, int *res);

