#include <stdlib.h>  // malloc, free
#include <time.h>    // struct timespec
#include <thread.h>
#include "thread_test_suite.h"


static int a = 0;

void increment(void *arg);
void increment(void *arg)
{
    (void) arg;
    a++;
}


TEST (thread_new_and_free_test) 
{
    struct thread *t = NULL;
    ASSERT (thread_new(NULL) != 0, "passing NULL to thread_new should fail");
    t = malloc(get_thread_data_size());
    ASSERT (thread_new(&t) != 0, "passing already allocated memory to thread_new should fail");
    free(t); t = NULL;
    ASSERT (thread_new(&t) == 0, "passing a non-initialized to thread_new should succeed");

    thread_free(&t);
    ASSERT (t == NULL, "struct contains NULL again after freeing");
    return 0;
}


TEST (thread_creation_and_destruction_test)
{
    struct thread *t = NULL;
    ASSERT (thread_create(NULL, NULL, NULL) != 0, "thread_create should fail if only NULL passed in");   
    ASSERT (thread_create(NULL, increment, NULL) != 0, "thread_create should fail if thread arg == NULL");   

    thread_new(&t);
    ASSERT (thread_create(t, increment, NULL) == 0, "creating thread should succeed");

    ASSERT (thread_create(t, NULL, NULL) != 0, "thread_create should fail if callback == NULL");   
    ASSERT(thread_join(t) == 0, "thread_join should succeed");
    ASSERT (a == 1, "variable should be modified by the thread");

    thread_free(&t);
    return 0;
}


TEST (thread_equal_test)
{
    struct thread *t1 = NULL;
    struct thread *t2 = NULL;
    ASSERT(thread_equal(NULL, NULL) != 0, "thread_equal should fail when NULLs passed in");
    ASSERT(thread_equal(t1, NULL) != 0, "thread_equal should fail when a NULL is passed in");
    ASSERT(thread_equal(NULL, t2) != 0, "thread_equal should fail when a NULL is passed in");

    thread_new(&t1);
    thread_new(&t2);
    thread_create(t1, increment, NULL);
    thread_create(t2, increment, NULL);
    ASSERT(thread_equal(t1, t1) != 0, "thread equal returns non-zero if they are equal");
    ASSERT(thread_equal(t1, t2) == 0, "thread equal returns zero if not equal");
    thread_join(t1);
    thread_join(t2);
    thread_free(&t1);
    thread_free(&t2);
    return 0;
}


TEST (thread_current_test)
{
    ASSERT(thread_current(NULL) != 0, "passing NULL to thread_current returns non-zero");
    thread_id_t id = 0;
    ASSERT(thread_current(&id) == 0, "thread_current should return 0 on success");
    return 0;
}


TEST (thread_detach_test)
{
    ASSERT(thread_detach(NULL) != 0, "passing NULL to thread_detach");
    struct thread *t = NULL;
    thread_new(&t);
    thread_create(t, increment, NULL);
    ASSERT(0 == thread_detach(t), "thread should successfully detach");
    struct timespec ms = { .tv_nsec = 1000000 };
    thread_sleep(&ms);
    // thread_free(&t);  - not needed; detached thread is auto cleaned up
    return 0;
}


TEST (thread_join_test)
{
    // good cases already tested in other tests
    ASSERT(thread_join(NULL) != 0, "passing NULL to thread_join should fail");
    return 0;
}


TEST (thread_test_suite)
{
    RUN_TEST(thread_new_and_free_test);
    RUN_TEST(thread_creation_and_destruction_test);
    RUN_TEST(thread_equal_test);
    RUN_TEST(thread_current_test);
    RUN_TEST(thread_detach_test);
    RUN_TEST(thread_join_test);
    return 0;
}

