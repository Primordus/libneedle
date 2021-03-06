#include <stdlib.h>            // malloc
#include <time.h>              // timespec
#include <mutex_internal.hpp>  // struct mutex (internal datatype structure)
#include <mutex.h>


int mutex_new(struct mutex **m)
{
    if (!m) { return 1; }
    if (*m) { return 2; }
    struct mutex *result = static_cast<struct mutex*>(malloc(get_mutex_data_size()));
    if (!result) { return 3; }
    *m = result;
    (*m)->is_initialized = false;
    return 0;
}


size_t get_mutex_data_size(void)
{
    return sizeof(struct mutex);
}


void mutex_free(struct mutex **m)
{
    if (!m) { return; }
    if (!*m) { return; }
    free(*m);
    *m = NULL;
}


int mutex_init(struct mutex *m, mutex_type type)
{
    if (!m) { return 1; }
    if (m->is_initialized) { return 2; }
    m->type = type;
    m->is_initialized = true;

    switch (type)
    {
    case PLAIN_MUTEX:
        new (&m->data.mtx) std::mutex;
        return 0;
    case RECURSIVE_MUTEX:
        new (&m->data) std::recursive_mutex;
        return 0;
    case TIMED_MUTEX:
        new (&m->data) std::timed_mutex;
        return 0; 
    case RECURSIVE_TIMED_MUTEX:
        new (&m->data) std::recursive_timed_mutex;
        return 0;
    }
    
    return 3;
}


int mutex_lock(struct mutex *m)
{
    if (!m) { return 1; }
    if (!m->is_initialized) { return 2; }

    switch (m->type)
    {
    case PLAIN_MUTEX:
        m->data.mtx.lock();
        return 0;
    case RECURSIVE_MUTEX:
        m->data.rec_mtx.lock();
        return 0;
    case TIMED_MUTEX:
        m->data.timed_mtx.lock();
        return 0; 
    case RECURSIVE_TIMED_MUTEX:
        m->data.rec_timed_mtx.lock();
        return 0;
    }

    return 3;
}


int mutex_timedlock(struct mutex *__restrict m, 
                    const struct timespec *__restrict timepoint)
{
    if (!m) { return 2; }
    if (!timepoint) { return 3; }
    if (!m->is_initialized) { return 4; }
    if (m->type == PLAIN_MUTEX || m->type == RECURSIVE_MUTEX) { return 5; }

    auto d = std::chrono::duration_cast<std::chrono::system_clock::duration>(
            std::chrono::seconds(timepoint->tv_sec)
          + std::chrono::nanoseconds(timepoint->tv_nsec));
    std::chrono::system_clock::time_point point_in_time(d);

    if (m->type == TIMED_MUTEX)
    {
        return !m->data.timed_mtx.try_lock_until(point_in_time);
    }
    // recursive timed mutex:
    return !m->data.rec_timed_mtx.try_lock_until(point_in_time);
}


int mutex_trylock(struct mutex *m)
{
    if (!m) { return 2; }
    if (!m->is_initialized) { return 3; }

    switch (m->type)
    {
    case PLAIN_MUTEX:
        return !m->data.mtx.try_lock();        
    case RECURSIVE_MUTEX:
        return !m->data.rec_mtx.try_lock();
    case TIMED_MUTEX:
        return !m->data.timed_mtx.try_lock();
    case RECURSIVE_TIMED_MUTEX:
        return !m->data.rec_timed_mtx.try_lock();
    }

    return 4;
}


int mutex_unlock(struct mutex *m)
{
    if (!m) { return 1; }
    if (!m->is_initialized) { return 2; }

    switch (m->type)
    {
    case PLAIN_MUTEX:
        m->data.mtx.unlock();        
        return 0;
    case RECURSIVE_MUTEX:
        m->data.rec_mtx.unlock();
        return 0;
    case TIMED_MUTEX:
        m->data.timed_mtx.unlock();
        return 0;
    case RECURSIVE_TIMED_MUTEX:
        m->data.rec_timed_mtx.unlock();
        return 0;
    }

    return 3;
}


void mutex_destroy(struct mutex *m)
{
    if (!m) { return; }
    if (!m->is_initialized) { return; }
    
    switch (m->type)
    {
    case PLAIN_MUTEX:
        m->data.mtx.~mutex();
        break;
    case RECURSIVE_MUTEX:
        m->data.rec_mtx.~recursive_mutex();
        break;
    case TIMED_MUTEX:
        m->data.timed_mtx.~timed_mutex();
        break;
    case RECURSIVE_TIMED_MUTEX:
        m->data.rec_timed_mtx.~recursive_timed_mutex();
        break;
    }

    m->is_initialized = false;
}

