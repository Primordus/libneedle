// Wrapper needed for making struct timespec available
#include <time.h>    // struct timespec
#include <stdlib.h>  // malloc, free
#include <thread.h>
#include <mutex.h>
#include <cond_var.h>
#include "cond_var_test_suite.h"


static struct cond_var *cv = NULL;
static struct mutex *m = NULL;
static struct thread *t = NULL;

static void make_dummy_thread(thread_start_t cb);
static void dummy_thread_cb(void *arg);
static void cleanup_cv(void);
static void cleanup_mutex(void);
static void cleanup_thread(void);
static void cleanup_all(void);


void make_dummy_thread(thread_start_t cb)
{
    thread_new(&t);
    thread_create(t, cb, &t);
}


void dummy_thread_cb(void *arg)
{
    (void)arg;
    struct timespec ms = { .tv_sec = 0, .tv_nsec = 1000000 };
    thread_sleep(&ms);
    cond_var_signal(cv);
}


void cleanup_cv(void)
{
    cond_var_destroy(cv);
    cond_var_free(&cv);
}


void cleanup_mutex(void)
{
    mutex_unlock(m);
    mutex_destroy(m);
    mutex_free(&m);
}


void cleanup_thread(void)
{
    thread_join(t);
    thread_free(&t);
}


void cleanup_all(void)
{
    cleanup_cv();
    cleanup_mutex();
    cleanup_thread();
}


TEST(cv_new)
{
    ASSERT(cond_var_new(NULL) != 0, "passing NULL to cond_var_new should fail");
    cv = malloc(get_cond_var_data_size());
    ASSERT(cond_var_new(&cv) != 0, "passing already allocated memory to cond_var_new should fail.");
    free(cv); cv = NULL;
    ASSERT(cond_var_new(&cv) == 0, "should return 0 on success.");

    cleanup_cv();
    return 0;
}


TEST(cv_free)
{
    cond_var_free(NULL);  // should be safe to pass in NULL
    cond_var_free(&cv);   // should be safe to pass in pointer to NULL

    cond_var_new(&cv);
    ASSERT(cv != NULL, "pointer should not be NULL after cond_var_new");
    cond_var_free(&cv);
    ASSERT(cv == NULL, "pointer should be set to NULL after freeing");
    return 0;
}


TEST(cv_init)
{
    ASSERT(cond_var_init(NULL) != 0, "passing NULL to init returns non-zero");
    cond_var_new(&cv);
    ASSERT(cond_var_init(cv) == 0, "returns 0 on success.");
    ASSERT(cond_var_init(cv) != 0, "returns non-zero if trying to initialize twice.");
    cond_var_free(&cv);
    cleanup_cv();
    return 0;
}


TEST(cv_signal)
{
    ASSERT(cond_var_signal(NULL) != 0, "passing NULL to signal returns non-zero");
    cond_var_new(&cv);
    ASSERT(cond_var_signal(cv) != 0, "returns non-zero if cv not initialized.");
    cond_var_init(cv);
    ASSERT(cond_var_signal(cv) == 0, "returns 0 on success.");
    cleanup_cv();
    return 0;
}


TEST(cv_broadcast)
{
    ASSERT(cond_var_broadcast(NULL) != 0, "passing NULL to broadcast returns non-zero");
    cond_var_new(&cv);
    ASSERT(cond_var_broadcast(cv) != 0, "returns non-zero if cv not initialized.");
    cond_var_init(cv);
    ASSERT(cond_var_broadcast(cv) == 0, "returns 0 on success.");
    cleanup_cv();
    return 0;
}


TEST(cv_wait)
{
    cond_var_new(&cv);
    mutex_new(&m);

    ASSERT(cond_var_wait(cv, m) != 0, "returns non-zero if cv and mutex not initialized");
    cond_var_init(cv);
    ASSERT(cond_var_wait(cv, m) != 0, "returns zero if mutex not initialized");
    mutex_init(m, PLAIN_MUTEX);

    ASSERT(cond_var_wait(NULL, NULL) != 0, "returns non-zero if NULLs passed in");
    ASSERT(cond_var_wait(NULL, m) != 0, "returns non-zero if a NULL is passed in");
    ASSERT(cond_var_wait(cv, NULL) != 0, "returns non-zero if a NULL is passed in");

    make_dummy_thread(dummy_thread_cb);
    ASSERT(cond_var_wait(cv, m) == 0, "returns zero on success");
    cleanup_all();

    cond_var_new(&cv);
    mutex_new(&m);
    mutex_init(m, PLAIN_MUTEX);
    ASSERT(cond_var_wait(cv, m) != 0, "returns zero if cond var not initialized");
    cleanup_all();
    return 0;
}


TEST(cv_timedwait)
{
    struct timespec two_ms;
    timespec_get(&two_ms, TIME_UTC);
    two_ms.tv_nsec += 2000000;

    cond_var_new(&cv);
    mutex_new(&m);
    ASSERT(cond_var_timedwait(cv, m, &two_ms) != 0, "returns non-zero if cond var and mutex not initialized");
    cond_var_init(cv);
    ASSERT(cond_var_timedwait(cv, m, &two_ms) != 0, "returns non-zero if mutex not initialized");
    cond_var_destroy(cv);
    mutex_init(m, TIMED_MUTEX);
    ASSERT(cond_var_timedwait(cv, m, &two_ms) != 0, "returns non-zero if cond var not initialized");
    cleanup_all();


    cond_var_new(&cv);
    mutex_new(&m);
    cond_var_init(cv);
    mutex_init(m, TIMED_MUTEX);

    ASSERT(cond_var_timedwait(NULL, NULL, NULL) != 0, "returns non-zero if NULLs passed in");
    ASSERT(cond_var_timedwait(NULL, m, NULL) != 0, "returns non-zero if a NULL is passed in");
    ASSERT(cond_var_timedwait(cv, NULL, NULL) != 0, "returns non-zero if a NULL is passed in");
    ASSERT(cond_var_timedwait(NULL, NULL, &two_ms) != 0, "returns non-zero if a NULL is passed in");
    ASSERT(cond_var_timedwait(cv, m, &two_ms) != 0, "returns non-zero on timeout");

    // 2ms have passed, need new timepoint
    timespec_get(&two_ms, TIME_UTC);
    two_ms.tv_nsec += 2000000;
    make_dummy_thread(dummy_thread_cb);
    ASSERT(cond_var_timedwait(cv, m, &two_ms) == 0, "returns zero on success");
    cleanup_all();

    // other mutex types

    cond_var_new(&cv);
    mutex_new(&m);
    cond_var_init(cv);
    mutex_init(m, RECURSIVE_TIMED_MUTEX);
    timespec_get(&two_ms, TIME_UTC);
    two_ms.tv_nsec += 2000000;
    make_dummy_thread(dummy_thread_cb);
    ASSERT(cond_var_timedwait(cv, m, &two_ms) == 0, "returns zero on success");
    cleanup_all();

    cond_var_new(&cv);
    mutex_new(&m);
    cond_var_init(cv);
    mutex_init(m, PLAIN_MUTEX);
    timespec_get(&two_ms, TIME_UTC);
    two_ms.tv_nsec += 2000000;
    make_dummy_thread(dummy_thread_cb);
    ASSERT(cond_var_timedwait(cv, m, &two_ms) == 0, "returns zero on success");
    cleanup_all();

    cond_var_new(&cv);
    mutex_new(&m);
    cond_var_init(cv);
    mutex_init(m, RECURSIVE_MUTEX);
    timespec_get(&two_ms, TIME_UTC);
    two_ms.tv_nsec += 2000000;
    make_dummy_thread(dummy_thread_cb);
    ASSERT(cond_var_timedwait(cv, m, &two_ms) == 0, "returns zero on success");
    cleanup_all();
    return 0;
}


TEST(cv_destroy)
{
    cond_var_destroy(NULL);  // safe to pass in NULL

    cond_var_new(&cv);
    cond_var_destroy(cv);  // safe to pass in uninitialized cond_var
    cond_var_init(cv);
    cond_var_destroy(cv);  // should work properly if cond_var passed in

    cleanup_cv();
    return 0;
}


TEST (cond_var_test_suite)
{
    RUN_TEST(cv_new);
    RUN_TEST(cv_free);
    RUN_TEST(cv_init);
    RUN_TEST(cv_signal);
    RUN_TEST(cv_broadcast);
    RUN_TEST(cv_wait);
    RUN_TEST(cv_timedwait);
    RUN_TEST(cv_destroy);
    return 0;
}

