#include <minunit.h>
#include <stdio.h>

#define NOT_USED(x) (void)x


int tests_run = 0;


TEST (thread_usage) 
{
    ASSERT(2 == 0, "reason goes here");
    return 0;
}


TEST (run_all_tests)
{
    RUN_TEST(thread_usage);
    return 0;
}


int main(int argc, char** argv)
{
    NOT_USED(argc); NOT_USED(argv);

    const char *err = run_all_tests();    
    printf("\nRan %d successful tests.\n", tests_run);

    if (err)
    {
        printf("%s\n", err);
        return 1;
    }
    
    printf("All tests passed!\n");
    return 0;
}

