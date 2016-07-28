#pragma once
#include <stdio.h>


/*
 * MinUnit testing framework.
 * Retrieved from: http://www.jera.com/techinfo/jtns/jtn002.html
 */

#define mu_assert(message, test) do { if (!(test)) return message; } while(0)
#define mu_run_test(test) do { \
        const char *message = test(); \
        if (message) return message; \
        tests_run++; \
    } while(0)
extern int tests_run;


/**
 * End of minunit testing framework; below are some extra helpers for
 * easily adding more unit tests.
 */

#define DO_STRINGIFY(x) #x
#define STRINGIFY(x) DO_STRINGIFY(x)

#define TEST(name_test) const char *name_test(void); \
                        const char *name_test(void)
#define RUN_TEST(test) mu_run_test(test); printf(".")
#define ASSERT(test, msg) mu_assert("Test failed: " STRINGIFY(test) " at " \
                                    __FILE__ ":" STRINGIFY(__LINE__) \
                                    " -> "  "\""  msg "\"", test)


