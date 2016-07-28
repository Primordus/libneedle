#include <thread.h>
#include <stdlib.h>   // malloc
#include <time.h>     // timespec


struct thread
{
    thrd_t data;
};


int thread_new(struct thread **t)
{
    if (!t) { return 1; }
    if (*t) { return 2; }
    struct thread* result = (struct thread*) malloc(get_thread_data_size());
    if (!result) { return 3; }
    *t = result;
    return 0;
}


size_t get_thread_data_size()
{
    return sizeof(struct thread);
}


void thread_free(struct thread **t)
{
    if (!t) { return; }
    free(*t);
    *t = NULL;
}


int thread_create(struct thread *t, thread_start_t callback, void *arg)
{
    if (!t) { return THREAD_NOT_VALID; }
    if (!callback) { return CALLBACK_NOT_VALID; }
    return thrd_create(&t->data, callback, arg);
}


int thread_equal(const struct thread *t1, const struct thread *t2)
{
    if (!t1) { return THREAD_NOT_VALID; }
    if (!t2) { return THREAD_NOT_VALID; }
    return thrd_equal(t1->data, t2->data);
}


int thread_current(struct thread **t)
{
    if (!t) { return THREAD_NOT_VALID; }
    (*t)->data = thrd_current();
    return 0;
}


int thread_sleep(const struct timespec *time_point, struct timespec *remaining)
{
    return thrd_sleep(time_point, remaining);
}


void thread_yield(void)
{
    thrd_yield();
}


_Noreturn void thread_exit(int result)
{
    thrd_exit(result);
}


int thread_detach(const struct thread *t)
{
    if (!t) { return THREAD_NOT_VALID; }
    return thrd_detach(t->data);
}


int thread_join(const struct thread *t, int *res)
{
    if (!t) { return THREAD_NOT_VALID; }
    return thrd_join(t->data, res);
}


