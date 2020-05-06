#include "test_suite.h"

#include<stdio.h>
#include <stdlib.h>
#include<string.h>

int test_case_init( test_case_t* self, test_suite_t* owner )
{
    int err = 0;
    
    memset( self, 0, sizeof( test_case_t ) );

    self->test_suite = owner;

    return err;
}


int test_case_name_set( test_case_t* self, const char* pszname )
{
    int err =0;

    strncpy( self->szname, pszname, 255 );

    return err;
}

char* test_case_name_get( test_case_t* self )
{
    return self->szname;
}

int test_case_run( test_case_t* self )
{
    int err = 0;

    printf( "\nTest case : %s : START\n", self->szname );

    self->f( self );
 
    printf( "Test Case : %s : %d/%d passed\n",self->szname,  self->npassed, self->npassed + self->nfailed );
    printf( "Test Case : %s : %d/%d failed\n", self->szname, self->nfailed, self->npassed + self->nfailed );
    printf( "Test case : %s : END\n", self->szname );

    return err;
}

int test_case_finish( test_case_t* self )
{
    int err = 0;

    return err;
}

int test_suite_add_test_case( test_suite_t* self, const char* pszname, TEST_CASE_FUNCTION f)
{
    int err = 0;

    test_case_t*  tc = NULL;

    self->ntestcases++;
    self->testcases = (test_case_t*)realloc( self->testcases, sizeof(test_case_t) * self->ntestcases );

    tc = self->testcases + self->ntestcases-1;

    test_case_init( tc, self );
    test_case_name_set( tc, pszname );
    tc->f = f;

    test_case_run( tc );

    return err;
}

int test_suite_init( test_suite_t* self, unsigned int ctrl_flags )
{
    self->ctrl_flags = ctrl_flags;

    self->ntestcases = 0;
    self->testcases = NULL;
    return 0;
}

static int _test_case_passed( test_case_t* self, int res, const char* expr, const char* pszfile, unsigned int lineno )
{
    int err = 0;

    self->npassed++;

    printf( "%s(%d) : %s : %s\n", pszfile, lineno, expr, "PASS" );

    return err;
}

static int _test_case_failed( test_case_t* self, int res, const char* expr, const char* pszfile, unsigned int lineno )
{
    int err = 0;

    self->nfailed++;

    printf( "%s(%d) : %s : %s\n", pszfile, lineno, expr, "FAIL" );

    if( !res && self->ctrl_flags & TEST_SUITE_CTRL_EXIT_ON_FAIL )
        exit(1);

    return err;
}



int test_case_test( test_case_t* self, int res, const char* expr, const char* file, unsigned int lineno )
{
    if( res )
        _test_case_passed( self, res, expr, file, lineno );
    else
        _test_case_failed( self, res, expr, file, lineno );

    return 0;
}

int test_case_equal_int( test_case_t* self, int val, int expected, const char* pszfile, unsigned int lineno )
{
    int res = (val == expected);

    char buffer[ 512];

    sprintf( buffer, "%d == %d", val, expected );

    if( res )
        _test_case_passed( self, res, buffer, pszfile, lineno );
    else
        _test_case_failed( self, res, buffer, pszfile, lineno );

    return 0;
}

int test_case_equal_float( test_case_t* self, float val, float expected, const char* pszfile, unsigned int lineno )
{
    int res = (val == expected);

    char buffer[ 512];

    sprintf( buffer, "%f == %f", val, expected );

    if( res )
        _test_case_passed( self, res, buffer, pszfile, lineno );
    else
        _test_case_failed( self, res, buffer, pszfile, lineno );

    return 0;
}

int test_case_equal_double( test_case_t* self, double val, double expected, const char* pszfile, unsigned int lineno )
{
    int res = (val == expected);

    char buffer[ 512];

    sprintf( buffer, "%f == %f", val, expected );

    if( res )
        _test_case_passed( self, res, buffer, pszfile, lineno );
    else
        _test_case_failed( self, res, buffer, pszfile, lineno );

    return 0;
}


int test_suite_finish( test_suite_t* self )
{
    size_t i = 0;


    int npassed=0;
    int nfailed=0;

    for( i = 0; i < self->ntestcases; ++i )
    {
        test_case_t* tc = self->testcases + i;
    
        npassed += tc->npassed;
        nfailed += tc->nfailed;

        test_case_finish( tc );
    }

    printf( "\n" );
    printf( "%d/%d tests failed\n", nfailed, nfailed+npassed );
    printf( "%d/%d tests passed\n", npassed, nfailed+npassed );
    printf( "%d test cases\n", self->ntestcases );

    free( self->testcases );

    return 0;
}


