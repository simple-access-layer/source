#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include "csal/csal.h"

#define TF_TEST( tf, expr ) test_frame_test( tf, expr, #expr, __FILE__, __LINE__ )


enum TEST_CTRL_FLAGS
{
    TEST_CTRL_NONE=0
    , TEST_CTRL_EXIT_ON_FAIL=1
    , TEST_CTRL_VERBOSE=2
};

typedef struct _test_frame_t
{
    unsigned int ctrl_flags;
    int npassed;
    int nfailed;

} test_frame_t;

int test_frame_init( test_frame_t* self, unsigned int ctrl_flags )
{
    self->ctrl_flags = ctrl_flags;
    self->npassed = 0;
    self->nfailed = 0;
    return 0;
}

int test_frame_test( test_frame_t* self, int res, const char* expr, const char* file, unsigned int lineno )
{
    const char* pszres = res ? "PASS" : "FAIL";

    if( res ) self->npassed++;
    else self->nfailed++;


    if( self->ctrl_flags & (TEST_CTRL_VERBOSE|TEST_CTRL_EXIT_ON_FAIL) || !res )
        printf( "%s(%d) : %s : %s\n", file, lineno, expr, pszres );

    if( !res && self->ctrl_flags & TEST_CTRL_EXIT_ON_FAIL )
        exit(1);

    return 0;
}

int test_frame_finish( test_frame_t* self )
{
    printf( "%d/%d tests failed\n", self->nfailed, self->nfailed+self->npassed );
    return 0;
}


int float_cmp( float val1, float val2, float tol )
{
    return val1-val2 < tol;
}

#define FLOAT_COMPARE(x,y) float_cmp(x,y,0.001)

int test_constants( test_frame_t* tf )
{
#if 0
    TF_TEST( tf, 0==strcmp( csal_attr_int8_str, "int8" ) );
    TF_TEST( tf, 0==strcmp( csal_attr_int16_str, "int16" ) );
    TF_TEST( tf, 0==strcmp( csal_attr_int32_str, "int32" ) );
    TF_TEST( tf, 0==strcmp( csal_attr_int64_str, "int64" ) );

    TF_TEST( tf, 0==strcmp( csal_attr_uint8_str, "uint8" ) );
    TF_TEST( tf, 0==strcmp( csal_attr_uint16_str, "uint16" ) );
    TF_TEST( tf, 0==strcmp( csal_attr_uint32_str, "uint32" ) );
    TF_TEST( tf, 0==strcmp( csal_attr_uint64_str, "uint64" ) );
 
    TF_TEST( tf, 0==strcmp( csal_attr_float32_str, "float32" ) );
    TF_TEST( tf, 0==strcmp( csal_attr_float64_str, "float64" ) );
    TF_TEST( tf, 0==strcmp( csal_attr_string_str, "string" ) );
    TF_TEST( tf, 0==strcmp( csal_attr_branch_str, "branch" ) );
#endif
    return 0;
}

int test_construction( test_frame_t* tf )
{
    csal_attrib_uint8_t* at_uint8 = csal_attrib_uint8_create(8);
    csal_attrib_uint16_t* at_uint16 = csal_attrib_uint16_create(16);
    csal_attrib_uint32_t* at_uint32 = csal_attrib_uint32_create(32);
    csal_attrib_uint64_t* at_uint64 = csal_attrib_uint64_create(64);
 
    csal_attrib_int8_t* at_int8 = csal_attrib_int8_create(8);
    csal_attrib_int16_t* at_int16 = csal_attrib_int16_create(16);
    csal_attrib_int32_t* at_int32 = csal_attrib_int32_create(32);
    csal_attrib_int64_t* at_int64 = csal_attrib_int64_create(64);
    csal_attrib_float_t* at_float = csal_attrib_float_create(1.1);
    csal_attrib_double_t* at_double = csal_attrib_double_create(2.2);
    csal_attrib_string_t* at_string = csal_attrib_string_create( "a string");

#if 0
    csal_attrib_branch_t* at_branch = csal_attrib_branch_create();
#endif
    CSAL_ATTRIBUTE_TYPE at_int8_type = csal_attrib_type( (csal_attrib_t*)at_int8 );

    TF_TEST( tf, CSAL_ATTR_UINT8 == csal_attrib_type( (csal_attrib_t*)at_uint8) );
    TF_TEST( tf, CSAL_ATTR_UINT16 == csal_attrib_type( (csal_attrib_t*)at_uint16 ) );
    TF_TEST( tf, CSAL_ATTR_UINT32 == csal_attrib_type( (csal_attrib_t*)at_uint32 ) );
    TF_TEST( tf, CSAL_ATTR_UINT64 == csal_attrib_type( (csal_attrib_t*)at_uint64 ) );
    TF_TEST( tf, CSAL_ATTR_FLOAT32 == csal_attrib_type( (csal_attrib_t*)at_float ) );
    TF_TEST( tf, CSAL_ATTR_FLOAT64 == csal_attrib_type( (csal_attrib_t*)at_double ) );

    TF_TEST( tf, CSAL_ATTR_INT8 == at_int8_type );
    TF_TEST( tf, CSAL_ATTR_INT16 == csal_attrib_type( (csal_attrib_t*)at_int16 ) );
    TF_TEST( tf, CSAL_ATTR_INT32 == csal_attrib_type( (csal_attrib_t*)at_int32 ) );
    TF_TEST( tf, CSAL_ATTR_INT64 == csal_attrib_type( (csal_attrib_t*)at_int64 ) );
    TF_TEST( tf, CSAL_ATTR_STRING == csal_attrib_type( (csal_attrib_t*)at_string ) );
#if 0
    TF_TEST( tf, CSAL_ATTR_BRANCH == csal_attrib_type( (csal_attrib_t*)at_branch ) );
#endif
    TF_TEST( tf, 8 == csal_attrib_uint8_value_get( at_uint8 ) );
    TF_TEST( tf, 16 == csal_attrib_uint16_value_get( at_uint16 ) );
    TF_TEST( tf, 32 == csal_attrib_uint32_value_get( at_uint32 ) );
    TF_TEST( tf, 64 == csal_attrib_uint64_value_get( at_uint64 ) );
    
    TF_TEST( tf, FLOAT_COMPARE( 1.1, csal_attrib_float_value_get( at_float ) ) );
    TF_TEST( tf, FLOAT_COMPARE( 1.1, csal_attrib_double_value_get( at_double ) ) );


    TF_TEST( tf, 8 == csal_attrib_int8_value_get( at_int8 ) );
    TF_TEST( tf, 16 == csal_attrib_int16_value_get( at_int16 ) );
    TF_TEST( tf, 32 == csal_attrib_int32_value_get( at_int32 ) );
    TF_TEST( tf, 64 == csal_attrib_int64_value_get( at_int64 ) );

    TF_TEST( tf, 0 == csal_attrib_int8_value_set( at_int8, 1 ) );
    TF_TEST( tf, 0 == csal_attrib_int16_value_set( at_int16, 2 ) );
    TF_TEST( tf, 0 == csal_attrib_int32_value_set( at_int32, 3 ) );
    TF_TEST( tf, 0 == csal_attrib_int64_value_set( at_int64, 4 ) );
    TF_TEST( tf, 0 == csal_attrib_float_value_set( at_float, -1.1 ) );
    TF_TEST( tf, 0 == csal_attrib_double_value_set( at_double, -2.2 ) );

    TF_TEST( tf, 1 == csal_attrib_int8_value_get( at_int8 ) );
    TF_TEST( tf, 2 == csal_attrib_int16_value_get( at_int16 ) );
    TF_TEST( tf, 3 == csal_attrib_int32_value_get( at_int32 ) );
    TF_TEST( tf, 4 == csal_attrib_int64_value_get( at_int64 ) );
    TF_TEST( tf, FLOAT_COMPARE( -1.1, csal_attrib_float_value_get( at_float ) ) );
    TF_TEST( tf, FLOAT_COMPARE( -2.2, csal_attrib_double_value_get( at_double ) ) );



    TF_TEST( tf, 0 == csal_attrib_uint8_value_set( at_uint8, 1 ) );
    TF_TEST( tf, 0 == csal_attrib_uint16_value_set( at_uint16, 2 ) );
    TF_TEST( tf, 0 == csal_attrib_uint32_value_set( at_uint32, 3 ) );
    TF_TEST( tf, 0 == csal_attrib_uint64_value_set( at_uint64, 4 ) );

    TF_TEST( tf, 1 == csal_attrib_uint8_value_get( at_uint8 ) );
    TF_TEST( tf, 2 == csal_attrib_uint16_value_get( at_uint16 ) );
    TF_TEST( tf, 3 == csal_attrib_uint32_value_get( at_uint32 ) );
    TF_TEST( tf, 4 == csal_attrib_uint64_value_get( at_uint64 ) );



    TF_TEST( tf, 0 == csal_attrib_destroy( (csal_attrib_t*)at_uint8 ) );
    TF_TEST( tf, 0 == csal_attrib_destroy( (csal_attrib_t*)at_uint16 ) );
    TF_TEST( tf, 0 == csal_attrib_destroy( (csal_attrib_t*)at_uint32 ) );
    TF_TEST( tf, 0 == csal_attrib_destroy( (csal_attrib_t*)at_uint64 ) );


    TF_TEST( tf, 0 == csal_attrib_destroy( (csal_attrib_t*)at_int8 ) );
    TF_TEST( tf, 0 == csal_attrib_destroy( (csal_attrib_t*)at_int16 ) );
    TF_TEST( tf, 0 == csal_attrib_destroy( (csal_attrib_t*)at_int32 ) );
    TF_TEST( tf, 0 == csal_attrib_destroy( (csal_attrib_t*)at_int64 ) );

    TF_TEST( tf, 0 == csal_attrib_destroy( (csal_attrib_t*)at_string ) );
#if 0
    TF_TEST( tf, 0 == csal_attrib_destroy( (csal_attrib_t*)at_branch ) );
#endif
    return 0;
}
#if 0
int test_branch( test_frame_t* tf )
{
    csal_attrib_int8_t* at_int8 = csal_attrib_int8_create(8);
    csal_attrib_int16_t* at_int16 = csal_attrib_int16_create(16);
    csal_attrib_int32_t* at_int32 = csal_attrib_int32_create(32);
    csal_attrib_int64_t* at_int64 = csal_attrib_int64_create(64);
    csal_attrib_string_t* at_string = csal_attrib_string_create( "a string");


    csal_attrib_branch_t* at_branch = csal_attrib_branch_create();


    csal_attrib_branch_set( at_branch, "key1", (csal_attrib_t*)at_int8 );
    csal_attrib_branch_set( at_branch, "key2", (csal_attrib_t*)at_int16 );
    csal_attrib_branch_set( at_branch, "key32", (csal_attrib_t*)at_int32 );


    TF_TEST( tf, csal_true == csal_attrib_branch_has( at_branch, "key1" ) );
    TF_TEST( tf, csal_false == csal_attrib_branch_has( at_branch, "nokey" ) );

    csal_attrib_t* at_got = csal_attrib_branch_get( at_branch, "key1" );
    TF_TEST( tf, NULL != at_got );

    TF_TEST( tf, CSAL_ATTR_INT8 == csal_attrib_type( at_got ) ) ;
    TF_TEST( tf, 8==csal_attrib_int8_value_get( (csal_attrib_int8_t*)at_got ) );
    
    TF_TEST( tf, 0 == csal_attrib_branch_remove( at_branch, "key1" ) );

    TF_TEST( tf, csal_false == csal_attrib_branch_has( at_branch, "key1" ) );

    TF_TEST( tf, "ds1" && 0 == csal_attrib_destroy( (csal_attrib_t*)at_int8 ) );

    TF_TEST( tf, "ds2" && 0 == csal_attrib_destroy( (csal_attrib_t*)at_int16 ) );

    TF_TEST( tf, 0 == csal_attrib_destroy( (csal_attrib_t*)at_int32 ) );

    TF_TEST( tf, 0 == csal_attrib_destroy( (csal_attrib_t*)at_int64 ) );
    TF_TEST( tf, 0 == csal_attrib_destroy( (csal_attrib_t*)at_string ) );


    TF_TEST( tf, 0 == csal_attrib_destroy( (csal_attrib_t*)at_branch ) );
    return 0;
}
#endif
int test_int8_array( test_frame_t* tf )
{

	csal_attrib_array_t* csal_array_ptr = NULL;

	int csal_err = 0;
    uint64_t dims[2];
    
    uint64_t shape[2];
    uint64_t nshape;

    uint64_t idxs[2];
    uint64_t ndims=2;

    int i,j;

    dims[0]=3;
    dims[1]=4;
    csal_attrib_array_int8_t* at_arr = csal_attrib_array_int8_create( dims, ndims );
    







    TF_TEST( tf, NULL != at_arr );

    TF_TEST( tf, CSAL_ATTR_ARRAY == csal_attrib_type( (csal_attrib_t*)at_arr ) );

    csal_err = csal_attrib_array_shape( (csal_attrib_array_t*)(at_arr), shape, &nshape, 2 );


    TF_TEST(tf, 0 == csal_err );
    TF_TEST(tf, 2 == nshape );



    csal_err = csal_attrib_array_shape( (csal_attrib_array_t*)(at_arr), shape, &nshape, 1 );

    TF_TEST(tf, 1 == csal_err );

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            csal_attrib_array_int8_element_set( at_arr, idxs, ndims, i*j);
        }
    }

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            TF_TEST( tf, i*j==csal_attrib_array_int8_element_get( at_arr, idxs, ndims) );
        }
    }




    csal_err = csal_attrib_array_cast( (csal_attrib_array_t*)at_arr, IID_CSAL_ATTRIB_ARRAY_INT8, (void**)(&csal_array_ptr) );

    TF_TEST( tf, 0 == csal_err );
    TF_TEST( tf, NULL != csal_array_ptr );

    csal_err = csal_attrib_array_cast( (csal_attrib_array_t*)at_arr, IID_CSAL_ATTRIB_ARRAY_INT16, (void**)(&csal_array_ptr) );

    TF_TEST( tf, 1 == csal_err );
    TF_TEST( tf, NULL == csal_array_ptr );

    csal_err = csal_attrib_array_cast( (csal_attrib_array_t*)at_arr, IID_CSAL_ATTRIB_ARRAY_INT32, (void**)(&csal_array_ptr) );

    TF_TEST( tf, 1 == csal_err );
    TF_TEST( tf, NULL == csal_array_ptr );

    csal_err = csal_attrib_array_cast( (csal_attrib_array_t*)at_arr, IID_CSAL_ATTRIB_ARRAY_INT64, (void**)(&csal_array_ptr) );

    TF_TEST( tf, 1 == csal_err );
    TF_TEST( tf, NULL == csal_array_ptr );

    csal_err = csal_attrib_array_cast( (csal_attrib_array_t*)at_arr, IID_CSAL_ATTRIB_ARRAY_UINT64, (void**)(&csal_array_ptr) );

    TF_TEST( tf, 1 == csal_err );
    TF_TEST( tf, NULL == csal_array_ptr );



    csal_attrib_destroy( (csal_attrib_t*)(at_arr) );

    return 0;
}
int test_int16_array( test_frame_t* tf )
{

    uint64_t dims[2];
    uint64_t idxs[2];
    uint64_t ndims=2;

    int i,j;
    dims[0]=3;
    dims[1]=4;

    csal_attrib_array_int16_t* at_arr = csal_attrib_array_int16_create( dims, ndims );

    TF_TEST( tf, NULL != at_arr );
    TF_TEST( tf, CSAL_ATTR_ARRAY == csal_attrib_type( (csal_attrib_t*)at_arr ) );

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            csal_attrib_array_int16_element_set( at_arr, idxs, ndims, i*j);
        }
    }

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            TF_TEST( tf, i*j==csal_attrib_array_int16_element_get( at_arr, idxs, ndims) );
        }
    }

    csal_attrib_destroy( (csal_attrib_t*)(at_arr) );
    return 0;
}

int test_int32_array( test_frame_t* tf )
{
    uint64_t dims[2];
    uint64_t idxs[2];
    uint64_t ndims=2;

    int i,j;
    int csal_err = 0;

    dims[0]=3;
    dims[1]=4;

    csal_attrib_array_int32_t* at_arr = csal_attrib_array_int32_create( dims, ndims );
    csal_attrib_array_int32_t* csal_array_ptr = NULL;


    TF_TEST( tf, NULL != at_arr );
    TF_TEST( tf, CSAL_ATTR_ARRAY == csal_attrib_type( (csal_attrib_t*)at_arr ) );

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            csal_attrib_array_int32_element_set( at_arr, idxs, ndims, i*j);
        }
    }

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            TF_TEST( tf, i*j==csal_attrib_array_int32_element_get( at_arr, idxs, ndims) );
        }
    }
    
    csal_err = csal_attrib_array_cast( (csal_attrib_array_t*)at_arr, IID_CSAL_ATTRIB_ARRAY_INT32, (void**)(&csal_array_ptr) );

    TF_TEST( tf, 0 == csal_err );
    TF_TEST( tf, NULL != csal_array_ptr );


    csal_attrib_destroy( (csal_attrib_t*)(at_arr) );
    return 0;
}

int test_int64_array( test_frame_t* tf )
{
    uint64_t dims[2];
    uint64_t idxs[2];
    uint64_t ndims=2;

    int i,j;

    dims[0]=3;
    dims[1]=4;

    csal_attrib_array_int64_t* at_arr = csal_attrib_array_int64_create( dims, ndims );

    TF_TEST( tf, NULL != at_arr );
    TF_TEST( tf, CSAL_ATTR_ARRAY == csal_attrib_type( (csal_attrib_t*)at_arr ) );

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            csal_attrib_array_int64_element_set( at_arr, idxs, ndims, i*j);
        }
    }

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            TF_TEST( tf, i*j==csal_attrib_array_int64_element_get( at_arr, idxs, ndims) );
        }
    }

    csal_attrib_destroy( (csal_attrib_t*)(at_arr) );
    return 0;
}





int test_uint8_array( test_frame_t* tf )
{
    uint64_t dims[2];
    uint64_t idxs[2];
    uint64_t ndims=2;

    int i,j;

    dims[0]=3;
    dims[1]=4;

    csal_attrib_array_uint8_t* at_arr = csal_attrib_array_uint8_create( dims, ndims );


    TF_TEST( tf, NULL != at_arr );
    TF_TEST( tf, CSAL_ATTR_ARRAY == csal_attrib_type( (csal_attrib_t*)at_arr ) );

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            csal_attrib_array_uint8_element_set( at_arr, idxs, ndims, i*j);
        }
    }

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            TF_TEST( tf, i*j==csal_attrib_array_uint8_element_get( at_arr, idxs, ndims) );
        }
    }

    csal_attrib_destroy( (csal_attrib_t*)(at_arr) );
    return 0;
}
int test_uint16_array( test_frame_t* tf )
{
    uint64_t dims[2];
    uint64_t idxs[2];
    uint64_t ndims=2;

    int i,j;

    dims[0]=3;
    dims[1]=4;

    csal_attrib_array_uint16_t* at_arr = csal_attrib_array_uint16_create( dims, ndims );

    TF_TEST( tf, NULL != at_arr );
    TF_TEST( tf, CSAL_ATTR_ARRAY == csal_attrib_type( (csal_attrib_t*)at_arr ) );

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            csal_attrib_array_uint16_element_set( at_arr, idxs, ndims, i*j);
        }
    }

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            TF_TEST( tf, i*j==csal_attrib_array_uint16_element_get( at_arr, idxs, ndims) );
        }
    }

    csal_attrib_destroy( (csal_attrib_t*)(at_arr) );
    return 0;
}

int test_uint32_array( test_frame_t* tf )
{
    uint64_t dims[2];
    uint64_t idxs[2];
    uint64_t ndims=2;

    int i,j;

    dims[0]=3;
    dims[1]=4;

    csal_attrib_array_uint32_t* at_arr = csal_attrib_array_uint32_create( dims, ndims );

    TF_TEST( tf, NULL != at_arr );
    TF_TEST( tf, CSAL_ATTR_ARRAY == csal_attrib_type( (csal_attrib_t*)at_arr ) );

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            csal_attrib_array_uint32_element_set( at_arr, idxs, ndims, i*j);
        }
    }

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            TF_TEST( tf, i*j==csal_attrib_array_uint32_element_get( at_arr, idxs, ndims) );
        }
    }
    
    csal_attrib_destroy( (csal_attrib_t*)(at_arr) );
    return 0;
}

int test_uint64_array( test_frame_t* tf )
{
    uint64_t dims[2];
    uint64_t idxs[2];
    uint64_t ndims=2;

    int i,j;

    dims[0]=3;
    dims[1]=4;

    csal_attrib_array_uint64_t* at_arr = csal_attrib_array_uint64_create( dims, ndims );

    TF_TEST( tf, NULL != at_arr );
    TF_TEST( tf, CSAL_ATTR_ARRAY == csal_attrib_type( (csal_attrib_t*)at_arr ) );

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            csal_attrib_array_uint64_element_set( at_arr, idxs, ndims, i*j);
        }
    }

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            TF_TEST( tf, i*j==csal_attrib_array_uint64_element_get( at_arr, idxs, ndims) );
        }
    }

    csal_attrib_destroy( (csal_attrib_t*)(at_arr) );
    return 0;
}




int test_float32_array( test_frame_t* tf )
{
    uint64_t dims[2];
    uint64_t idxs[2];
    uint64_t ndims=2;

    int i,j;

    dims[0]=3;
    dims[1]=4;

    csal_attrib_array_float32_t* at_arr = csal_attrib_array_float32_create( dims, ndims );

    TF_TEST( tf, NULL != at_arr );
    TF_TEST( tf, CSAL_ATTR_ARRAY == csal_attrib_type( (csal_attrib_t*)at_arr ) );

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            csal_attrib_array_float32_element_set( at_arr, idxs, ndims, i*j);
        }
    }

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            TF_TEST( tf, i*j==csal_attrib_array_float32_element_get( at_arr, idxs, ndims) );
        }
    }

    csal_attrib_destroy( (csal_attrib_t*)(at_arr) );
    return 0;
}


int test_float64_array( test_frame_t* tf )
{
    uint64_t dims[2];
    uint64_t idxs[2];
    uint64_t ndims=2;

    int i,j;

    dims[0]=3;
    dims[1]=4;

    csal_attrib_array_float64_t* at_arr = csal_attrib_array_float64_create( dims, ndims );

    TF_TEST( tf, NULL != at_arr );
    TF_TEST( tf, CSAL_ATTR_ARRAY == csal_attrib_type( (csal_attrib_t*)at_arr ) );

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            csal_attrib_array_float64_element_set( at_arr, idxs, ndims, i*j);
        }
    }

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
            idxs[0] = i;
            idxs[1] = j;

            TF_TEST( tf, i*j==csal_attrib_array_float64_element_get( at_arr, idxs, ndims) );
        }
    }

    csal_attrib_destroy( (csal_attrib_t*)(at_arr) );

    return 0;
}


int test_string_array( test_frame_t* tf )
{
    uint64_t dims[2];
    uint64_t idxs[2];
    uint64_t ndims=2;

    char buffer[ 256 ];

    int i,j;

    dims[0]=3;
    dims[1]=4;

    csal_attrib_array_string_t* at_arr = csal_attrib_array_string_create( dims, ndims );

    TF_TEST( tf, NULL != at_arr );
    TF_TEST( tf, CSAL_ATTR_ARRAY == csal_attrib_type( (csal_attrib_t*)at_arr ) );

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
	    sprintf( buffer, "%d", i*j );
            idxs[0] = i;
            idxs[1] = j;

            csal_attrib_array_string_element_set( at_arr, idxs, ndims, buffer );
        }
    }

    for( i = 0; i < dims[0]; ++i )
    {
        for( j = 0; j < dims[1]; ++j )
        {
		sprintf( buffer, "%d", i*j );
            idxs[0] = i;
            idxs[1] = j;

            TF_TEST( tf, 0 == strcmp( buffer, csal_attrib_array_string_element_get( at_arr, idxs, ndims) ) );
        }
    }

    csal_attrib_destroy( (csal_attrib_t*)(at_arr) );
    return 0;
}


























int main()
{
    test_frame_t tf;

    test_frame_init( &tf, TEST_CTRL_VERBOSE|TEST_CTRL_EXIT_ON_FAIL );

    test_constants( &tf );

    test_construction( &tf );
#if 0
    test_branch( &tf );
#endif

    if( 1 )
    {
	    test_int8_array( &tf );
	    test_int16_array( &tf );
	    test_int32_array( &tf );
	    test_int64_array( &tf );

	    test_uint8_array( &tf );
	    test_uint16_array( &tf );
	    test_uint32_array( &tf );
	    test_uint64_array( &tf );

	    test_float32_array( &tf );
	    test_float64_array( &tf );
	    test_string_array( &tf );
    }
    test_frame_finish( &tf );

    return 0;
}
