#ifndef __CSAL_ATTRIB_H__
#define __CSAL_ATTRIB_H__

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _csal_attrib_t csal_attrib_t;
 
    typedef struct _csal_attrib_uint8_t csal_attrib_uint8_t;
    typedef struct _csal_attrib_uint16_t csal_attrib_uint16_t;
    typedef struct _csal_attrib_uint32_t csal_attrib_uint32_t;
    typedef struct _csal_attrib_uint64_t csal_attrib_uint64_t;

   
    typedef struct _csal_attrib_int8_t csal_attrib_int8_t;
    typedef struct _csal_attrib_int16_t csal_attrib_int16_t;
    typedef struct _csal_attrib_int32_t csal_attrib_int32_t;
    typedef struct _csal_attrib_int64_t csal_attrib_int64_t;

    typedef struct _csal_attrib_float_t csal_attrib_float_t;
    typedef struct _csal_attrib_double_t csal_attrib_double_t;
#if 0
    typedef struct _csal_attrib_branch_t csal_attrib_branch_t;
#endif

    typedef struct _csal_scalar_attrib_t csal_scalar_attrib_t;
    typedef struct _csal_attrib_string_t csal_attrib_string_t;
    typedef struct _csal_attrib_array_t csal_attrib_array_t;
#if 0
    typedef struct _csal_attrib_branch_t csal_attrib_branch_t;
#endif
    
    //typedef struct _csal_attrib_array_t csal_attrib_array_t;
    typedef struct _csal_attrib_array_int8_t csal_attrib_array_int8_t;
    typedef struct _csal_attrib_array_int16_t csal_attrib_array_int16_t;
    typedef struct _csal_attrib_array_int32_t csal_attrib_array_int32_t;
    typedef struct _csal_attrib_array_int64_t csal_attrib_array_int64_t;

    typedef struct _csal_attrib_array_uint8_t csal_attrib_array_uint8_t;
    typedef struct _csal_attrib_array_uint16_t csal_attrib_array_uint16_t;
    typedef struct _csal_attrib_array_uint32_t csal_attrib_array_uint32_t;
    typedef struct _csal_attrib_array_uint64_t csal_attrib_array_uint64_t;


    typedef struct _csal_attrib_array_string_t csal_attrib_array_string_t;

    typedef struct _csal_attrib_array_float32_t csal_attrib_array_float32_t;
    typedef struct _csal_attrib_array_float64_t csal_attrib_array_float64_t;


    CSAL_ATTRIBUTE_TYPE csal_attrib_type( csal_attrib_t* self );

    csal_attrib_array_int8_t* csal_attrib_array_int8_create( uint64_t* _shape, uint64_t nshapes );
    csal_attrib_array_int16_t* csal_attrib_array_int16_create( uint64_t* _shape, uint64_t nshapes );
    csal_attrib_array_int32_t* csal_attrib_array_int32_create( uint64_t* _shape, uint64_t nshapes );
    csal_attrib_array_int64_t* csal_attrib_array_int64_create( uint64_t* _shape, uint64_t nshapes );



csal_attrib_array_uint8_t* csal_attrib_array_uint8_create( uint64_t* _shape, uint64_t nshapes );
    csal_attrib_array_uint16_t* csal_attrib_array_uint16_create( uint64_t* _shape, uint64_t nshapes );
    csal_attrib_array_uint32_t* csal_attrib_array_uint32_create( uint64_t* _shape, uint64_t nshapes );
    csal_attrib_array_uint64_t* csal_attrib_array_uint64_create( uint64_t* _shape, uint64_t nshapes );

    
    csal_attrib_array_float32_t* csal_attrib_array_float32_create( uint64_t* _shape, uint64_t nshapes );
    csal_attrib_array_float64_t* csal_attrib_array_float64_create( uint64_t* _shape, uint64_t nshapes );
    csal_attrib_array_string_t* csal_attrib_array_string_create( uint64_t* _shape, uint64_t nshapes );




    csal_attrib_uint8_t* csal_attrib_uint8_create( uint8_t val );
    csal_attrib_uint16_t* csal_attrib_uint16_create( uint16_t val );
    csal_attrib_uint32_t* csal_attrib_uint32_create( uint32_t val );
    csal_attrib_uint64_t* csal_attrib_uint64_create( uint64_t val );
 
    csal_attrib_int8_t* csal_attrib_int8_create( int8_t val );
    csal_attrib_int16_t* csal_attrib_int16_create( int16_t val );
    csal_attrib_int32_t* csal_attrib_int32_create( int32_t val );
    csal_attrib_int64_t* csal_attrib_int64_create( int64_t val );

    csal_attrib_float_t* csal_attrib_float_create( float val );
    csal_attrib_double_t* csal_attrib_double_create( double val );

    uint8_t csal_attrib_uint8_value_get( csal_attrib_uint8_t* self );
    uint16_t csal_attrib_uint16_value_get( csal_attrib_uint16_t* self );
    uint32_t csal_attrib_uint32_value_get( csal_attrib_uint32_t* self );
    uint64_t csal_attrib_uint64_value_get( csal_attrib_uint64_t* self );

    int8_t csal_attrib_int8_value_get( csal_attrib_int8_t* self );
    int16_t csal_attrib_int16_value_get( csal_attrib_int16_t* self );
    int32_t csal_attrib_int32_value_get( csal_attrib_int32_t* self );
    int64_t csal_attrib_int64_value_get( csal_attrib_int64_t* self );

    float csal_attrib_float_value_get( csal_attrib_float_t* self );
    double csal_attrib_double_value_get( csal_attrib_double_t* self );

    int csal_attrib_uint8_value_set( csal_attrib_uint8_t* self, uint8_t val );
    int csal_attrib_uint16_value_set( csal_attrib_uint16_t* self, uint16_t val );
    int csal_attrib_uint32_value_set( csal_attrib_uint32_t* self, uint32_t val );
    int csal_attrib_uint64_value_set( csal_attrib_uint64_t* self, uint64_t val );

    int csal_attrib_int8_value_set( csal_attrib_int8_t* self, int8_t val );
    int csal_attrib_int16_value_set( csal_attrib_int16_t* self, int16_t val );
    int csal_attrib_int32_value_set( csal_attrib_int32_t* self, int32_t val );
    int csal_attrib_int64_value_set( csal_attrib_int64_t* self, int64_t val );
    int csal_attrib_float_value_set( csal_attrib_float_t* self, float val );
    int csal_attrib_double_value_set( csal_attrib_double_t* self, double val );


    int csal_attrib_destroy( csal_attrib_t* self );

    csal_attrib_string_t* csal_attrib_string_create( const char* pszval );
/*    int csal_string_attrib_destroy( csal_string_attrib_t* self );*/


//    csal_array_attrib_t* csal_array_attrib_int16_create( uint64_t* shape, uint64_t nshapes );
 //   csal_array_attrib_t* csal_array_attrib_int32_create( uint64_t* shape, uint64_t nshapes );
  //  csal_array_attrib_t* csal_array_attrib_int64_create( uint64_t* shape, uint64_t nshapes );
   // csal_array_attrib_t* csal_array_attrib_float_create( uint64_t* shape, uint64_t nshapes );
 //   csal_array_attrib_t* csal_array_attrib_double_create( uint64_t* shape, uint64_t nshapes );
  //  csal_array_attrib_t* csal_array_attrib_string_create( uint64_t* shape, uint64_t nshapes );



    int8_t csal_attrib_array_int8_element_get( csal_attrib_array_int8_t* self, uint64_t* indices, uint64_t nindices );
    int16_t csal_attrib_array_int16_element_get( csal_attrib_array_int16_t* self, uint64_t* indices, uint64_t nindices );
    int32_t csal_attrib_array_int32_element_get( csal_attrib_array_int32_t* self, uint64_t* indices, uint64_t nindices );
    int64_t csal_attrib_array_int64_element_get( csal_attrib_array_int64_t* self, uint64_t* indices, uint64_t nindices );

    uint8_t csal_attrib_array_uint8_element_get( csal_attrib_array_uint8_t* self, uint64_t* indices, uint64_t nindices );
    uint16_t csal_attrib_array_uint16_element_get( csal_attrib_array_uint16_t* self, uint64_t* indices, uint64_t nindices );
    uint32_t csal_attrib_array_uint32_element_get( csal_attrib_array_uint32_t* self, uint64_t* indices, uint64_t nindices );
    uint64_t csal_attrib_array_uint64_element_get( csal_attrib_array_uint64_t* self, uint64_t* indices, uint64_t nindices );


#if 1
    const char* csal_attrib_array_string_element_get( csal_attrib_array_string_t* self, uint64_t* indices, uint64_t nindices );
    float csal_attrib_array_float32_element_get( csal_attrib_array_float32_t* self, uint64_t* indices, uint64_t nindices );
    double csal_attrib_array_float64_element_get( csal_attrib_array_float64_t* self, uint64_t* indices, uint64_t nindices );
#endif



    int csal_attrib_array_int8_element_set( csal_attrib_array_int8_t* self, uint64_t* indices, uint64_t nindices, int8_t val );
    int csal_attrib_array_int16_element_set( csal_attrib_array_int16_t* self, uint64_t* indices, uint64_t nindices, int16_t val );
    int csal_attrib_array_int32_element_set( csal_attrib_array_int32_t* self, uint64_t* indices, uint64_t nindices, int32_t val );
    int csal_attrib_array_int64_element_set( csal_attrib_array_int64_t* self, uint64_t* indices, uint64_t nindices, int64_t val );


    int csal_attrib_array_uint8_element_set( csal_attrib_array_uint8_t* self, uint64_t* indices, uint64_t nindices, uint8_t val );
    int csal_attrib_array_uint16_element_set( csal_attrib_array_uint16_t* self, uint64_t* indices, uint64_t nindices, uint16_t val );
    int csal_attrib_array_uint32_element_set( csal_attrib_array_uint32_t* self, uint64_t* indices, uint64_t nindices, uint32_t val );
    int csal_attrib_array_uint64_element_set( csal_attrib_array_uint64_t* self, uint64_t* indices, uint64_t nindices, uint64_t val );


    int csal_attrib_array_string_element_set( csal_attrib_array_string_t* self, uint64_t* indices, uint64_t nindices, const char* val );
    int csal_attrib_array_float32_element_set( csal_attrib_array_float32_t* self, uint64_t* indices, uint64_t nindices, float val );
    int csal_attrib_array_float64_element_set( csal_attrib_array_float64_t* self, uint64_t* indices, uint64_t nindices, double val );


    const char* csal_attrib_array_eltype( csal_attrib_array_t* self );


    int csal_attrib_array_shape( csal_attrib_array_t* self, uint64_t* vals, uint64_t* nvals, uint64_t nval_max );


#ifdef __cplusplus
}
#endif




#endif
