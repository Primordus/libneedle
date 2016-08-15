#include <stdlib.h>   // malloc
#include <time.h>     // timespec
#include <threads.h>  // mtx_t
#include <mutex.h>


struct mutex
{
    mtx_t data;  // note, keeps this member of struct first (for casting)!
};


int mutex_new(struct mutex **m)
{
    if (!m) { return 1; }
    if (*m) { return 2; }
    struct mutex *result = (struct mutex*) malloc(get_mutex_data_size());
    if (!result) { return 3; }
    *m = result;
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
    if (!m) return 1;
    switch (type)
    {
    case PLAIN:
        return mtx_init(&m->data, mtx_plain);
    case RECURSIVE:
        return mtx_init(&m->data, mtx_recursive);
    case TIMED:
        return mtx_init(&m->data, mtx_timed);
    }
}


int mutex_lock(struct mutex *m)
{
    if (!m) { return 1; }
    mtx_lock(&m->data);
    return 0;
}


int mutex_timedlock(struct mutex *restrict m, 
                    const struct timespec *restrict timepoint)
{
    if (!m) { return 1; }
    return mtx_timedlock(&m->data, timepoint);
}


int mutex_trylock(struct mutex *m)
{
    if (!m) { return 1; }
    return mtx_trylock(&m->data);
}


int mutex_unlock(struct mutex *m)
{
    if (!m) { return 1; }
    return mtx_unlock(&m->data);
}


void mutex_destroy(struct mutex *m)
{
    if (!m) { return; }
    mtx_destroy(&m->data);
}

