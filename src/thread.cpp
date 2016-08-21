#include <thread>     // std::thread
#include <stdlib.h>   // malloc
#include <time.h>     // timespec
#include <chrono>     // std::chrono::*
#include <thread.h>


struct thread
{
    std::thread data;
    bool is_initialized;
    uint8_t dummy_padding[7];  // silences -Wpadded
};


int thread_new(struct thread **t)
{
    if (!t) { return 1; }
    if (*t) { return 2; }
    struct thread *result = static_cast<struct thread*>(malloc(get_thread_data_size()));
    if (!result) { return 3; }
    *t = result;
    (*t)->is_initialized = false;
    return 0;
}


size_t get_thread_data_size(void)
{
    return sizeof(struct thread);
}


void thread_free(struct thread **t)
{
    if (!t) { return; }
    if (!(*t)) { return; }
    if ((*t)->is_initialized) { (*t)->data.~thread(); }
    free(*t);
    *t = NULL;
}


int thread_create(struct thread *t, thread_start_t callback, void *arg)
{
    if (!t) { return 1; }
    if (!callback) { return 2; }
    if (t->is_initialized) { return 3; }
    new (&t->data) std::thread(callback, arg);
    t->is_initialized = true;
    return 0;
}


int thread_equal(const struct thread * const t1, const struct thread * const t2)
{
    if (!t1) { return -1; }
    if (!t2) { return -2; }
    if (!t1->is_initialized) { return -3; }
    if (!t2->is_initialized) { return -4; }
    return t1->data.get_id() == t2->data.get_id();
}


int thread_current(thread_id_t *id)
{
    if (!id) { return 1; }
    std::hash<std::thread::id> hasher;
    *id = hasher(std::this_thread::get_id());
    return 0;
}


int thread_sleep(const struct timespec *time_point)
{
    if (!time_point) { return 1; }
    auto d = std::chrono::duration_cast<std::chrono::system_clock::duration>(
            std::chrono::seconds(time_point->tv_sec)
          + std::chrono::nanoseconds(time_point->tv_nsec));
    std::chrono::system_clock::time_point point_in_time(d);
    std::this_thread::sleep_until(point_in_time);
    return 0;
}


void thread_yield(void)
{
    std::this_thread::yield();
}


int thread_detach(struct thread *t)
{
    if (!t) { return 1; }
    if (!t->is_initialized) { return 2; }
    t->data.detach();
    return 0;
}


int thread_join(struct thread *t)
{
    if (!t) { return 1; }
    if (!t->is_initialized) { return 2; }
    t->data.join();
    return 0;
}

