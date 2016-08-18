#pragma once

#ifdef __cplusplus
extern "C" 
{
#endif


/**
 * Wrapper API around mtx_t.
 * Provides a few helper functions for letting the caller allocate memory the
 * way he wants it for storing the mutex.
 *
 * For the rest API is the same as C11 API.
 */


typedef enum
{
    PLAIN_MUTEX,
    RECURSIVE_MUTEX,
    TIMED_MUTEX,
    RECURSIVE_TIMED_MUTEX
} mutex_type;


struct mutex;
struct timespec;

/**
 * Helper functions for allocating / freeing a mutex
 */

int mutex_new(struct mutex **m);
size_t get_mutex_data_size(void);
void mutex_free(struct mutex **m);


/**
 * Wrapper functions around mtx_t API.
 */


int mutex_init(struct mutex *m, mutex_type type);
int mutex_lock(struct mutex *m);
int mutex_timedlock(struct mutex *__restrict m, 
                    const struct timespec *__restrict timepoint);
int mutex_trylock(struct mutex *m);
int mutex_unlock(struct mutex *m);
void mutex_destroy(struct mutex *m);


#ifdef __cplusplus
}
#endif

