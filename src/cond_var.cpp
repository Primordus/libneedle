#include <stdlib.h>            // malloc, free
#include <mutex_internal.hpp>  // mutex (internal structure)
#include <condition_variable>  // std::condition_variable_any
#include <cond_var.h>



struct cond_var
{
    // Uses _any since we don't know type of lock ahead of time
    std::condition_variable_any data;
    bool is_initialized;
    uint8_t dummy[7];  // silences -Wpadded error.
};


int cond_var_new(struct cond_var **cv)
{
    if (!cv) { return 1; }
    if (*cv) { return 2;}
    struct cond_var *result = static_cast<struct cond_var*>(malloc(get_cond_var_data_size()));
    if (!result) { return 3; }
    *cv = result;
    (*cv)->is_initialized = false;
    return 0;
}


size_t get_cond_var_data_size(void)
{
    return sizeof(struct cond_var);
}


void cond_var_free(struct cond_var **cv)
{
    if (!cv) { return; }
    if (!(*cv)) { return; }
    free(*cv);
    *cv = NULL;
}


int cond_var_init(struct cond_var *cv)
{
    if (!cv) { return 1; }
    if (cv->is_initialized) { return 2; }
    new (&cv->data) std::condition_variable_any;
    cv->is_initialized = true;
    return 0;
}


int cond_var_signal(struct cond_var *cv)
{
    if (!cv) { return 1; }
    if (!cv->is_initialized) { return 2; }
    cv->data.notify_one();
    return 0;
}


int cond_var_broadcast(struct cond_var *cv)
{
    if (!cv) { return 1; }
    if (!cv->is_initialized) { return 2; }
    cv->data.notify_all();
    return 0;
}


int cond_var_wait(struct cond_var *cv, struct mutex *m)
{
    if (!cv) { return 1; }
    if (!m) { return 2; }
    if (!cv->is_initialized) { return 3; }
    if (!m->is_initialized) { return 4; }

    switch (m->type)
    {
    case PLAIN_MUTEX:
        {
            std::unique_lock<std::mutex> lock(m->data.mtx);
            cv->data.wait(lock);
        }
        return 0;
    case RECURSIVE_MUTEX:
        {
            std::unique_lock<std::recursive_mutex> lock(m->data.rec_mtx);
            cv->data.wait(lock);
        }
        return 0;
    case TIMED_MUTEX:
        {
            std::unique_lock<std::timed_mutex> lock(m->data.timed_mtx);
            cv->data.wait(lock);
        }
        return 0;
    case RECURSIVE_TIMED_MUTEX:
        {
            std::unique_lock<std::recursive_timed_mutex> lock(m->data.rec_timed_mtx);
            cv->data.wait(lock);
        }
        return 0;
    }
}


int cond_var_timedwait(struct cond_var *__restrict cv, struct mutex *__restrict m,
                       const struct timespec *__restrict timepoint)
{
    if (!cv) { return 1; }
    if (!m) { return 2; }
    if (!cv->is_initialized) { return 3; }
    if (!m->is_initialized) { return 4; }
    if (!timepoint) { return 5; }

    auto d = std::chrono::duration_cast<std::chrono::system_clock::duration>(
             std::chrono::seconds(timepoint->tv_sec)
           + std::chrono::nanoseconds(timepoint->tv_nsec));
    std::chrono::system_clock::time_point point_in_time(d);

    switch (m->type)
    {
    case PLAIN_MUTEX:
        {
            std::unique_lock<std::mutex> lock(m->data.mtx);
            return cv->data.wait_until(lock, point_in_time) == std::cv_status::timeout;
        }
    case RECURSIVE_MUTEX:
        {
            std::unique_lock<std::recursive_mutex> lock(m->data.rec_mtx);
            return cv->data.wait_until(lock, point_in_time) == std::cv_status::timeout;
        }
    case TIMED_MUTEX:
        {
            std::unique_lock<std::timed_mutex> lock(m->data.timed_mtx);
            return cv->data.wait_until(lock, point_in_time) == std::cv_status::timeout;
        }
    case RECURSIVE_TIMED_MUTEX:
        {
            std::unique_lock<std::recursive_timed_mutex> lock(m->data.rec_timed_mtx);
            return cv->data.wait_until(lock, point_in_time) == std::cv_status::timeout;
        }
    }
}


void cond_var_destroy(struct cond_var *cv)
{
    if (!cv) { return; }
    if (!cv->is_initialized) { return; }
    cv->data.~condition_variable_any();
    cv->is_initialized = false;
}

