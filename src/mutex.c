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
    struct mutex *result = (struct mutex*) malloc(get_mutex_data_size());
    if (!result) { return 1; }
    *m = result;
    return 0;
}


size_t get_mutex_data_size(void)
{
    return sizeof(struct mutex);
}


void mutex_free(struct mutex *m)
{
    free(m);
}


int mutex_init(struct mutex *m, mutex_type type)
{
    if (!m) return MUTEX_NOT_VALID;
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


int mutex_lock(struct mutex *m);
int mutex_timedlock(struct mutex *restrict mutex, 
                    const struct timespec *restrict timepoint);
int mutex_trylock(struct mutex *m);
int mutex_unlock(struct mutex *m);
void mutex_destroy(struct mutex *m);


