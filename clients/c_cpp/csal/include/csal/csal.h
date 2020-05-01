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

    typedef struct _csal_object_t csal_object_t;
    typedef struct _csal_report_t csal_report_t; 





    /* sal::node namespace wrappers */
    typedef struct _csal_node_leaftype_t csal_node_leaftype_t;

    typedef struct _csal_node_report_t csal_node_report_t;

    typedef struct _csal_node_object_t csal_node_object_t;

    typedef struct _csal_node_branch_t csal_node_branch_t;

    typedef struct _csal_node_leaf_t csal_node_leaf_t;


#if 0

    typedef struct _csal_node_client_t csal_node_client_t;
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

#if 0
    csal_attrib_branch_t* csal_attrib_branch_create(void);

    csal_attrib_t* csal_attrib_branch_get( csal_attrib_branch_t* self, const char* pszkey );

    int csal_attrib_branch_set( csal_attrib_branch_t* self, const char* pszkey, csal_attrib_t* _at );

    csal_bool csal_attrib_branch_has( csal_attrib_branch_t* self, const char* pszkey );
    int csal_attrib_branch_remove( csal_attrib_branch_t* self, const char* pszkey );
#endif

    




#ifdef __cplusplus
}
#endif


#include "csal/detail/csal_client.h"
#include "csal/detail/csal_attrib.h"

#ifdef __cplusplus
//#include "sal.h"

//csal_attrib_t* csal_attrib_from_sal( sal::object::Attribute::Ptr sal_ptr );
//sal::object::Attribute::Ptr sal_attrib_from_csal( csal_attrib_t* csal_ptr );
#endif


#endif

