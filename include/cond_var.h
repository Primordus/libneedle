#pragma once

/**
 * Helper module which provides a wrapper API around C11 cnd_t.
 * Provides convenience functions for the caller to allocate the condition
 * variable the way they want.
 *
 * For the rest API is the same as C11 cnd_t API.
 */


struct timespec;
struct mutex;
struct cond_var;


/**
 * Helper functions for allocating and freeing a condition variable.
 */


int cond_var_new(struct cond_var **cv);
size_t get_cond_var_data_size(void);
void cond_var_free(struct cond_var **cv);


/**
 * Wrapper API around c11 cnd_t API.
 */


int cond_var_init(struct cond_var *cv);
int cond_var_signal(struct cond_var *cv);
int cond_var_broadcast(struct cond_var *cv);
int cond_var_wait(struct cond_var *cv, struct mutex* m);
int cond_var_timedwait(struct cond_var *restrict cv, struct mutex *restrict m,
                       const struct timespec *restrict time_point);
void cond_var_destroy(struct cond_var* cv);


