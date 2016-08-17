#pragma once
#ifndef __cplusplus
#error "This header file is only parsable by a C++ compiler!"
#endif

#include <mutex>    // std::*mutex
#include <mutex.h>  // enum mutex_type


struct mutex
{
    union
    {
        std::mutex mtx;
        std::recursive_mutex rec_mtx;
        std::timed_mutex timed_mtx;
        std::recursive_timed_mutex rec_timed_mtx;
    } data;
    mutex_type type;
    bool is_initialized;
    uint8_t dummy[3];  // used to silence -Wpadded
};


