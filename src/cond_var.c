#include <stdlib.h>     // malloc, free
#include <threads.h>    // cnd_t
#include <mutex.h>      // mutex
#include <cond_var.h>



struct cond_var
{
    cnd_t data;
};


int cond_var_new(struct cond_var **cv)
{
    if (!cv) { return 1; }
    if (*cv) { return 2;}
    struct cond_var *result = (struct cond_var*) malloc(get_cond_var_data_size());
    if (!result) { return 3; }
    *cv = result;
    return 0;
}


size_t get_cond_var_data_size(void)
{
    return sizeof(struct cond_var);
}


void cond_var_free(struct cond_var **cv)
{
    if (!cv) { return; }
    if (!*cv) { return; }
    free(*cv);
    *cv = NULL;
}


int cond_var_init(struct cond_var *cv)
{
    if (!cv) { return 1; }
    return cnd_init(&cv->data);
}


int cond_var_signal(struct cond_var *cv)
{
    if (!cv) { return 1; }
    return cnd_signal(&cv->data);
}


int cond_var_broadcast(struct cond_var *cv)
{
    if (!cv) { return 1; }
    return cnd_broadcast(&cv->data);
}


int cond_var_wait(struct cond_var *cv, struct mutex* m)
{
    if (!cv) { return 1; }
    if (!m) { return 2; }
    return cnd_wait(&cv->data, (mtx_t*) m);
}


int cond_var_timedwait(struct cond_var *restrict cv, struct mutex *restrict m,
                       const struct timespec *restrict time_point)
{
    if (!cv) { return 1; }
    if (!m) { return 2; }
    return cnd_timedwait(&cv->data, (mtx_t*) m, time_point);
}


void cond_var_destroy(struct cond_var *cv)
{
    if (!cv) { return; }
    cnd_destroy(&cv->data);
}

