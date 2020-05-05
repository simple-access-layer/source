#ifndef __CSAL_ATTRIB_H__
#define __CSAL_ATTRIB_H__

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _csal_attrib_t csal_attrib_t;

    typedef const char csal_uuid_t;

#define IID_CSAL_ATTRIB "5bda9c61-3c3f-48a2-9993-dd869c8942ce"


 


#define IID_CSAL_ATTRIB_BOOL "ba14054d-8a87-4d9a-bad9-1786917e4cc2"
#define IID_CSAL_ATTRIB_UINT8 "81ca1837-b676-41c4-aaf0-04732d5dde05"
#define IID_CSAL_ATTRIB_UINT16 "b3929122-b113-444c-90ae-024a7d147a5d"
#define IID_CSAL_ATTRIB_UINT32 "c6d7da6e-e2c0-496e-8ee2-ad969321e806"
#define IID_CSAL_ATTRIB_UINT64 "e1bcfb9c-ae77-42c5-aec6-3914903c8975"

#define IID_CSAL_ATTRIB_INT8 "79c23346-b981-443d-8ec4-b949004ed874"
#define IID_CSAL_ATTRIB_INT16 "40bc1f12-384c-4c3d-86fb-26609e9ad4f8"
#define IID_CSAL_ATTRIB_INT32 "775678d0-8bea-4f2b-954a-122fbf36a772"
#define IID_CSAL_ATTRIB_INT64 "c2483fd6-3e35-4745-81fd-6438b61bf4a0"

#define IID_CSAL_ATTRIB_FLOAT32 "be41cab6-9c25-44a0-9584-aed0f403e643"
#define IID_CSAL_ATTRIB_FLOAT64 "783a6af1-218f-4b5f-a001-28201f5e8ba0"

#define IID_CSAL_ATTRIB_STRING "fcbbf072-b6ad-479d-8b4b-31b181c5e153"


#define IID_CSAL_ATTRIB_ARRAY "05ad26ad-1f46-49ca-808d-bd30b162cb74"


#define IID_CSAL_ATTRIB_ARRAY_INT8 "bc089d10-6f34-4ac1-ae43-63c8fc6e33e4"
#define IID_CSAL_ATTRIB_ARRAY_INT16 "2048c2cb-a34b-4177-9bca-1468bebdb501"
#define IID_CSAL_ATTRIB_ARRAY_INT32 "c5ba1b60-afc6-41b0-b6bb-0c123e1a3d1f"
#define IID_CSAL_ATTRIB_ARRAY_INT64 "b9a26a11-4b5f-46d6-89cc-bc0c21e89b52"

#define IID_CSAL_ATTRIB_ARRAY_UINT8 "b19fc6d0-eafe-4a95-9b9c-f40d30972487"
#define IID_CSAL_ATTRIB_ARRAY_UINT16 "1411a80e-9840-4601-a7ac-d97eda520ec6"
#define IID_CSAL_ATTRIB_ARRAY_UINT32 "e6e009b8-e0a7-4f3a-a01d-fa2c1d0e54b5"
#define IID_CSAL_ATTRIB_ARRAY_UINT64 "07b31421-79be-4539-8ed6-b42ef9e1b19f"

#define IID_CSAL_ATTRIB_ARRAY_FLOAT32 "0066ceef-ee31-4a6e-a0be-94fbd9c1647c"
#define IID_CSAL_ATTRIB_ARRAY_FLOAT64 "59b78402-1957-45f0-b59f-64533df3c07d"
#define IID_CSAL_ATTRIB_ARRAY_STRING "9ae687c2-0404-48a0-af1a-cbf1528bb70b"

    typedef struct _csal_attrib_bool_t csal_attrib_bool_t;

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


    int csal_attrib_cast( csal_attrib_t* self, csal_uuid_t* at_type, void** ppv );

    csal_bool_t csal_attrib_is_type( csal_attrib_t* self, CSAL_ATTRIBUTE_TYPE attrib_type );
#if 0
    int csal_attrib_array_cast( csal_attrib_array_t* self, csal_uuid_t* csal_at_type, void** ppv );
#endif


    int csal_attrib_int8_cast( csal_attrib_int8_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_int16_cast( csal_attrib_int16_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_int32_cast( csal_attrib_int32_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_int64_cast( csal_attrib_int64_t* self, csal_uuid_t* at_type, void** ppv );

    int csal_attrib_uint8_cast( csal_attrib_uint8_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_uint16_cast( csal_attrib_uint16_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_uint32_cast( csal_attrib_uint32_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_uint64_cast( csal_attrib_uint64_t* self, csal_uuid_t* at_type, void** ppv );


    int csal_attrib_float32_cast( csal_attrib_float_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_float64_cast( csal_attrib_double_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_string_cast( csal_attrib_string_t* self, csal_uuid_t* at_type, void** ppv );

    int csal_attrib_array_int8_cast( csal_attrib_array_int8_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_array_int16_cast( csal_attrib_array_int16_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_array_int32_cast( csal_attrib_array_int32_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_array_int64_cast( csal_attrib_array_int64_t* self, csal_uuid_t* at_type, void** ppv );

    int csal_attrib_array_uint8_cast( csal_attrib_array_uint8_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_array_uint16_cast( csal_attrib_array_uint16_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_array_uint32_cast( csal_attrib_array_uint32_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_array_uint64_cast( csal_attrib_array_uint64_t* self, csal_uuid_t* at_type, void** ppv );

    int csal_attrib_array_float32_cast( csal_attrib_array_float32_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_array_float64_cast( csal_attrib_array_float64_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_array_string_cast( csal_attrib_array_string_t* self, csal_uuid_t* at_type, void** ppv );


#if 0
    int csal_attrib_uint8_cast( csal_attrib_uint8_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_uint16_cast( csal_attrib_uint16_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_uint32_cast( csal_attrib_uint32_t* self, csal_uuid_t* at_type, void** ppv );
    int csal_attrib_uint64_cast( csal_attrib_uint64_t* self, csal_uuid_t* at_type, void** ppv );
#endif
    CSAL_ATTRIBUTE_TYPE csal_attrib_type( csal_attrib_t* self );

    int csal_attrib_array_shape( csal_attrib_array_t* self, uint64_t* vals, uint64_t* nvals, uint64_t nval_max );

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




    csal_attrib_bool_t* csal_attrib_bool_create( csal_bool_t val );
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
