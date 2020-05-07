#ifndef __TEST_SUITE_H__
#define __TEST_SUITE_H__

#include <setjmp.h>

#define TC_TEST( tf, expr ) test_case_test( tf, expr, #expr, __FILE__, __LINE__ )

#define TC_TEST_EQUAL_INT( tc, val, expected ) test_case_equal_int( tc, val, expected, __FILE__, __LINE__ )
#define TC_TEST_EQUAL_FLOAT( tc, val, expected ) test_case_equal_float( tc, val, expected, __FILE__, __LINE__ )
#define TC_TEST_EQUAL_DOUBLE( tc, val, expected ) test_case_equal_double( tc, val, expected, __FILE__, __LINE__ )

#define TS_TEST_CASE( tf, f ) test_suite_add_test_case( tf, #f, f )



#ifdef __cplusplus
extern "C"
{
#endif


struct _test_suite_t;
struct _test_case_t;

typedef int (*TEST_CASE_FUNCTION)( struct _test_case_t* );

enum TEST_SUITE_PROC_FLAGS
{
    TEST_SUITE_PROC_NONE=0
        , TEST_SUITE_PROC_RUN_TESTS=1
};

enum TEST_SUITE_CTRL_FLAGS
{
    TEST_SUITE_CTRL_NONE=0
        , TEST_SUITE_CTRL_EXIT_ON_FAIL=1
        , TEST_SUITE_CTRL_SUMMARY=2
        , TEST_SUITE_CTRL_REPORT_PASS=4
};

#define TEST_SUITE_CTRL_VERBOSE (TEST_SUITE_CTRL_REPORT_PASS|TEST_SUITE_CTRL_SUMMARY)
typedef struct test_result_t
{
    int passed;
    char szexp[ 512 ];
    char szfile[ 512 ];
    unsigned int lineno;

} test_result_t;


typedef struct _test_case_t
{
    TEST_CASE_FUNCTION f;
    char szname[ 256];
    int npassed;
    int nfailed;

    struct _test_suite_t* test_suite;

    size_t nresults;
    test_result_t* test_results;

} test_case_t;


typedef struct _test_suite_t
{
    unsigned int ctrl_flags;
    unsigned int processing_flags;
    int ntestcases;
    test_case_t* testcases;

    jmp_buf abort_jmp_buf;

} test_suite_t;



int test_case_init( test_case_t* self, test_suite_t* owner );
int test_case_name_set( test_case_t* self, const char* pszname );
char* test_case_name_get( test_case_t* self );

int test_case_run( test_case_t* self );

int test_case_finish( test_case_t* self );

int test_suite_add_test_case( test_suite_t* self, const char* pszname, TEST_CASE_FUNCTION f);



int test_suite_init( test_suite_t* self, unsigned int ctrl_flags );

int test_case_test( test_case_t* self, int res, const char* expr, const char* file, unsigned int lineno );

int test_case_equal_int( test_case_t* self, int val, int expected, const char* pszfile, unsigned int lineno );

int test_case_equal_float( test_case_t* self, float val, float expected, const char* pszfile, unsigned int lineno );

int test_case_equal_double( test_case_t* self, double val, double expected, const char* pszfile, unsigned int lineno );

int test_suite_finish( test_suite_t* self );
#ifdef __cplusplus
}
#endif


#endif
