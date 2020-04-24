#ifndef __CSAL_H___
#define __CSAL_H___

#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif


    typedef enum 
    {
            CSAL_ATTR_NULL,   ///!> empty (uninitialized, null state), JSON null type
            CSAL_ATTR_SCALAR, ///!> JSON scalar number type + boolean = SAL scalar class
            // consider: split dtype out as DTYPE
            CSAL_ATTR_INT8, ///!> JSON scalar number type
            CSAL_ATTR_INT16,
            CSAL_ATTR_INT32,
            CSAL_ATTR_INT64,
            CSAL_ATTR_UINT8,
            CSAL_ATTR_UINT16,
            CSAL_ATTR_UINT32,
            CSAL_ATTR_UINT64,
            CSAL_ATTR_FLOAT32,
            CSAL_ATTR_FLOAT64,
            CSAL_ATTR_BOOL, ///!> JSON bool type

            CSAL_ATTR_STRING, ///!> JSON string type, UTF8 encoding assumed
            CSAL_ATTR_ARRAY,  ///!> JSON array type, with same element type
#if 0
            // as there is element type member, these new array types may not necessary
            CSAL_ATTR_INT8_ARRAY, 
            CSAL_ATTR_INT16_ARRAY,
            CSAL_ATTR_INT32_ARRAY,
            CSAL_ATTR_INT64_ARRAY,
            CSAL_ATTR_UINT8_ARRAY,
            CSAL_ATTR_UINT16_ARRAY,
            CSAL_ATTR_UINT32_ARRAY,
            CSAL_ATTR_UINT64_ARRAY,
            CSAL_ATTR_FLOAT32_ARRAY,
            CSAL_ATTR_FLOAT64_ARRAY,
            CSAL_ATTR_STRING_ARRAY, /// JSON array type for string
#endif
            CSAL_ATTR_DICTIONARY, ///!> JSON object type, container of children json types
            CSAL_ATTR_SIGNAL     ///!> high level data model for SAL physical pulse signal
#if 0
        CSAL_ATTR_INT8

            , CSAL_ATTR_INT16
            , CSAL_ATTR_INT32
            , CSAL_ATTR_INT64
            , CSAL_ATTR_UINT8
            , CSAL_ATTR_UINT16
            , CSAL_ATTR_UINT32
            , CSAL_ATTR_UINT64
            , CSAL_ATTR_FLOAT32
            , CSAL_ATTR_FLOAT64
            , CSAL_ATTR_BOOL
            , CSAL_ATTR_STRING
            , CSAL_ATTR_INT8_ARRAY
            , CSAL_ATTR_INT16_ARRAY
            , CSAL_ATTR_INT32_ARRAY
            , CSAL_ATTR_INT64_ARRAY
            , CSAL_ATTR_UINT8_ARRAY
            , CSAL_ATTR_UINT16_ARRAY
            , CSAL_ATTR_UINT32_ARRAY
            , CSAL_ATTR_UINT64_ARRAY
            , CSAL_ATTR_FLOAT32_ARRAY
            , CSAL_ATTR_FLOAT64_ARRAY
            , CSAL_ATTR_STRING_ARRAY
            , CSAL_ATTR_BRANCH
#endif       
    } CSAL_ATTRIBUTE_TYPE;
#if 0
    extern const char* csal_attr_uint8_str;
    extern const char* csal_attr_uint16_str;
    extern const char* csal_attr_uint32_str;
    extern const char* csal_attr_uint64_str;

    extern const char* csal_attr_int8_str;
    extern const char* csal_attr_int16_str;
    extern const char* csal_attr_int32_str;
    extern const char* csal_attr_int64_str;
    extern const char* csal_attr_float32_str;
    extern const char* csal_attr_float64_str;
    extern const char* csal_attr_string_str;
    extern const char* csal_attr_branch_str;
#endif

    typedef enum {csal_false=0, csal_true=1} csal_bool_t;

    typedef struct _csal_client_t csal_client_t;
    typedef struct _csal_object_t csal_object_t;
    typedef struct _csal_report_t csal_report_t; 

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





    /* sal::node namespace wrappers */
    typedef struct _csal_node_leaftype_t csal_node_leaftype_t;

    typedef struct _csal_node_report_t csal_node_report_t;

    typedef struct _csal_node_object_t csal_node_object_t;

    typedef struct _csal_node_branch_t csal_node_branch_t;

    typedef struct _csal_node_leaf_t csal_node_leaf_t;

    typedef struct _csal_node_client_t csal_node_client_t;

#if 0
	sal::node::Client* sal_cient;
#endif




	typedef struct _csal_node_info_t csal_node_info_t;
	csal_node_info_t* csal_node_info_create( const char* pszcls, const char* pszgroup, int version );
	int csal_node_info_destroy( csal_node_info_t* self );




    /* node namespace */
    typedef enum
    {
        CSAL_NODE_TYPE_BRANCH
            , CSAL_NODE_TYPE_LEAF
    } CSAL_NODE_TYPE;


    typedef struct _csal_node_object_t csal_node_object_t;

    csal_node_object_t* csal_node_object_create( csal_node_info_t* node_info, const char* pszdescription, CSAL_NODE_TYPE node_type );

    int csal_node_object_destroy( csal_node_object_t* self );

    csal_bool_t csal_node_object_isleaf( csal_node_object_t* self );

    csal_bool_t csal_node_object_isbranch( csal_node_object_t* self );
#if 0
    typedef struct _csal_node_object_branch_t csal_node_object_branch_t;

    typedef struct _csal_node_object_leaf_t csal_node_object_leaf_t;
#endif
    /*CSAL_NODE_TYPE csal_node_object_type( csal_node_object_t* self );*/

#if 0
    csal_node_object_branch_t* csal_node_object_branch_create( const char* pszdescription );
    int csal_node_object_destroy( csal_node_object_t* self );
#endif


    typedef struct _csal_node_leaf_t csal_node_leaf_t;

    typedef struct _csal_node_report_t csal_node_report_t;

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

#if 0
    csal_attrib_branch_t* csal_attrib_branch_create(void);

    csal_attrib_t* csal_attrib_branch_get( csal_attrib_branch_t* self, const char* pszkey );

    int csal_attrib_branch_set( csal_attrib_branch_t* self, const char* pszkey, csal_attrib_t* _at );

    csal_bool csal_attrib_branch_has( csal_attrib_branch_t* self, const char* pszkey );
    int csal_attrib_branch_remove( csal_attrib_branch_t* self, const char* pszkey );
#endif

    




    int csal_client_create( csal_client_t** pself, const char* pszhost, csal_bool_t verify_https_cert );
    int csal_client_destroy( csal_client_t* self );

    csal_bool_t csal_client_is_auth_required( csal_client_t* self );

    int csal_client_authenticate( csal_client_t* self, const char* pszuser, const char* pszpasswd );

    csal_report_t* csal_client_report( csal_client_t* self );
    
    int csal_client_put( csal_client_t* self, const char* pszpath, const csal_object_t* obj );
    csal_object_t* csal_client_get( csal_client_t* self, const char* pszpath, csal_bool_t summary );
    int csal_client_copy( csal_client_t* self, const char* psztarget, const char* pszsource );
    int csal_client_del( csal_client_t* self, const char* pszpath );

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
//#include "sal.h"

//csal_attrib_t* csal_attrib_from_sal( sal::object::Attribute::Ptr sal_ptr );
//sal::object::Attribute::Ptr sal_attrib_from_csal( csal_attrib_t* csal_ptr );
#endif


#endif

