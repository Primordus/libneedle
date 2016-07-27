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
    struct cond_var *result = (struct cond_var*) malloc(get_cond_var_data_size());
    if (!result) { return 1; }
    *cv = result;
    return 0;
}


size_t get_cond_var_data_size(void)
{
    return sizeof(struct cond_var);
}


void cond_var_free(struct cond_var *cv)
{
    free(cv);
}


int cond_var_init(struct cond_var *cv)
{
    if (!cv) { return COND_VAR_NOT_VALID; }
    return cnd_init(&cv->data);
}


int cond_var_signal(struct cond_var *cv)
{
    if (!cv) { return COND_VAR_NOT_VALID; }
    return cnd_signal(&cv->data);
}


int cond_var_broadcast(struct cond_var *cv)
{
    if (!cv) { return COND_VAR_NOT_VALID; }
    return cnd_broadcast(&cv->data);
}


int cond_var_wait(struct cond_var *cv, struct mutex* m)
{
    if (!cv) { return COND_VAR_NOT_VALID; }
    if (!m) { return MUTEX_NOT_VALID; }
    return cnd_wait(&cv->data, (mtx_t*) m);
}


int cond_var_timedwait(struct cond_var *restrict cv, struct mutex *restrict m,
                       const struct timespec *restrict time_point)
{
    if (!cv) { return COND_VAR_NOT_VALID; }
    if (!m) { return MUTEX_NOT_VALID; }
    return cnd_timedwait(&cv->data, (mtx_t*) m, time_point);
}


void cond_var_destroy(struct cond_var* cv)
{
    if (!cv) { return; }
    cnd_destroy(&cv->data);
}

