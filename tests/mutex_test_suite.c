#include <stdlib.h>  // malloc, free
#include <time.h>  // struct timespec
#include <mutex.h>
#include "mutex_test_suite.h"


static struct mutex *m = NULL;
static struct timespec timepoint;

static void cleanup_mutex(void);
static struct timespec *get_timepoint(void);


void cleanup_mutex(void)
{
    mutex_destroy(m);
    mutex_free(&m);
}


// returns a timepoint 1 ms in the future
struct timespec *get_timepoint(void)
{
    timespec_get(&timepoint, TIME_UTC);
    timepoint.tv_nsec += 1000000;
    return &timepoint;
}


TEST (mutex_new_test) 
{
    ASSERT(mutex_new(NULL) != 0, "passing NULL to mutex_new should fail");
    m = malloc(get_mutex_data_size());
    ASSERT(mutex_new(&m) != 0, "passing already allocated memory to mutex_new should fail.");
    free(m); m = NULL;
    ASSERT(mutex_new(&m) == 0, "should return 0 on success.");
    ASSERT(mutex_new(&m) != 0, "trying to new twice should fail.");

    cleanup_mutex();
    return 0;
}


TEST (mutex_init_test) 
{
    ASSERT(mutex_init(NULL, PLAIN_MUTEX) != 0, "passing NULL to init returns non-zero");
    mutex_new(&m);
    ASSERT(mutex_init(m, PLAIN_MUTEX) == 0, "returns 0 on success.");
    ASSERT(mutex_init(m, PLAIN_MUTEX) != 0, "returns non-zero if trying to init twice.");
    cleanup_mutex();
    mutex_new(&m);
    ASSERT(mutex_init(m, RECURSIVE_MUTEX) == 0, "returns 0 on success.");
    cleanup_mutex();
    mutex_new(&m);
    ASSERT(mutex_init(m, TIMED_MUTEX) == 0, "returns 0 on success.");
    cleanup_mutex();
    mutex_new(&m);
    ASSERT(mutex_init(m, RECURSIVE_TIMED_MUTEX) == 0, "returns 0 on success.");
    cleanup_mutex();
    return 0;
}


TEST (mutex_free_test) 
{
    mutex_free(NULL);  // should be safe to pass in NULL
    mutex_free(&m);    // should be safe to pass in pointer to null

    mutex_new(&m);
    ASSERT(m != NULL, "pointer shouldnt be NULL after mutex_new");
    mutex_free(&m);
    ASSERT(m == NULL, "pointer should be set to NULL after freeing");
    return 0;
}


TEST (mutex_lock_test) 
{
    mutex_new(&m);
    ASSERT(mutex_lock(NULL) != 0, "lock returns non-zero if NULL passed in");
    ASSERT(mutex_lock(m) != 0, "lock returns non-zero if mutex not initialized");
    mutex_init(m, PLAIN_MUTEX);
    ASSERT(mutex_lock(m) == 0, "lock returns 0 on success");
    ASSERT(mutex_unlock(m) == 0, "unlock returns O on success");

    cleanup_mutex();
    return 0;
}


TEST (mutex_timedlock_test) 
{
    mutex_new(&m);
    ASSERT(mutex_timedlock(NULL, NULL) != 0, "timedlock returns non-zero if NULL passed in");
    ASSERT(mutex_timedlock(NULL, get_timepoint()) != 0, "timedlock returns non-zero if NULL passed in");
    ASSERT(mutex_timedlock(m, get_timepoint()) != 0, "timedlock returns non-zero if mutex not initialized");
    
    mutex_init(m, TIMED_MUTEX);
    ASSERT(mutex_timedlock(m, get_timepoint()) == 0, "timedlock returns 0 on success");
    ASSERT(mutex_unlock(m) == 0, "unlock returns O on success");
    cleanup_mutex();

    mutex_new(&m);
    mutex_init(m, RECURSIVE_TIMED_MUTEX);
    ASSERT(mutex_timedlock(m, get_timepoint()) == 0, "timedlock returns 0 on success");
    ASSERT(mutex_unlock(m) == 0, "unlock returns O on success");
    cleanup_mutex();

    mutex_new(&m);
    mutex_init(m, PLAIN_MUTEX);
    ASSERT(mutex_timedlock(m, get_timepoint()) != 0, "timedlock returns non-zero if wrong type of mutex passed in");
    cleanup_mutex();

    mutex_new(&m);
    mutex_init(m, RECURSIVE_MUTEX);
    ASSERT(mutex_timedlock(m, get_timepoint()) != 0, "timedlock returns non-zero if wrong type of mutex passed in");
    cleanup_mutex();
    return 0;
}


TEST (mutex_trylock_test) 
{
    mutex_new(&m);
    ASSERT(mutex_trylock(m) != 0, "trylock returns non-zero if mutex not initialized");
    
    mutex_init(m, TIMED_MUTEX);
    ASSERT(mutex_trylock(m) == 0, "trylock returns 0 on success");
    ASSERT(mutex_unlock(m) == 0, "unlock returns O on success");
    ASSERT(mutex_trylock(m) == 0, "trylock returns 0 on success");
    ASSERT(mutex_trylock(m) != 0, "trylock returns non zero if trying to lock twice");
    ASSERT(mutex_unlock(m) == 0, "unlock returns O on success");
    
    cleanup_mutex();
    return 0;
}


TEST (mutex_unlock_test)
{
    ASSERT(mutex_unlock(NULL) != 0, "unlock should return non zero if NULL passed in");
    
    mutex_new(&m);
    ASSERT(mutex_unlock(m) != 0, "unlock should return non-zero if mutex not initialized");

    mutex_init(m, PLAIN_MUTEX);
    ASSERT(mutex_unlock(m) == 0, "unlock should return zero if no lock was taken prior");

    mutex_lock(m);
    ASSERT(mutex_unlock(m) == 0, "should return 0 on successful unlocking");
    ASSERT(mutex_unlock(m) == 0, "unlock should return zero if no lock was taken prior");

    cleanup_mutex();
    return 0;
}


TEST (mutex_destroy_test)
{
    mutex_destroy(NULL);  // should be safe to pass in NULL

    mutex_new(&m);
    mutex_destroy(m);     // should be safe in uninitialized mutex

    mutex_init(m, PLAIN_MUTEX);
    mutex_destroy(m);  // should work properly if mutex passed in 
    mutex_destroy(m);  // should be safe to destroy mutex twice 

    cleanup_mutex();
    return 0;
}

TEST (mutex_test_suite)
{
    RUN_TEST(mutex_new_test);
    RUN_TEST(mutex_init_test);
    RUN_TEST(mutex_free_test);
    RUN_TEST(mutex_lock_test);
    RUN_TEST(mutex_timedlock_test);
    RUN_TEST(mutex_trylock_test);
    RUN_TEST(mutex_unlock_test);
    RUN_TEST(mutex_destroy_test);
    return 0;
}

