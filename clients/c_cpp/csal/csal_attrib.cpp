
#include "sal.h"
#include "csal.h"
//#include "sal.h"


#include <cstddef>
#include <cassert>

#if 0
const char* csal_attr_uint8_str = sal::object::ID_STR_UINT8;
const char* csal_attr_uint16_str = sal::object::ID_STR_UINT16;
const char* csal_attr_uint32_str = sal::object::ID_STR_UINT32;
const char* csal_attr_uint64_str = sal::object::ID_STR_UINT64;
const char* csal_attr_int8_str = sal::object::ID_STR_INT8;
const char* csal_attr_int16_str = sal::object::ID_STR_INT16;
const char* csal_attr_int32_str = sal::object::ID_STR_INT32;const char* csal_attr_int64_str = sal::object::ID_STR_INT64;

const char* csal_attr_float32_str = sal::object::ID_STR_FLOAT32;
const char* csal_attr_float64_str = sal::object::ID_STR_FLOAT64;

const char* csal_attr_string_str = sal::object::ID_STR_STRING;

const char* csal_attr_branch_str = sal::object::ID_STR_BRANCH;
#endif








struct _csal_attrib_t
{
    sal::object::Attribute::Ptr sal_at_ptr;
};

struct _csal_attrib_uint8_t
{
    _csal_attrib_t base;
};
struct _csal_attrib_uint16_t
{
    _csal_attrib_t base;
};
struct _csal_attrib_uint32_t
{
    _csal_attrib_t base;
};
struct _csal_attrib_uint64_t
{
    _csal_attrib_t base;
};

struct _csal_attrib_int8_t
{
    _csal_attrib_t base;
};
struct _csal_attrib_int16_t
{
    _csal_attrib_t base;
};
struct _csal_attrib_int32_t
{
    _csal_attrib_t base;
};
struct _csal_attrib_int64_t
{
    _csal_attrib_t base;
};

struct _csal_attrib_float_t
{
    _csal_attrib_t base;
};
struct _csal_attrib_double_t
{
    _csal_attrib_t base;
};

struct _csal_attrib_string_t
{
    _csal_attrib_t base;
};

struct _csal_attrib_array_int8_t
{
    _csal_attrib_t base;
};

struct _csal_attrib_array_int16_t
{
    _csal_attrib_t base;
};

struct _csal_attrib_array_int32_t
{
    _csal_attrib_t base;
};

struct _csal_attrib_array_int64_t
{
    _csal_attrib_t base;
};


struct _csal_attrib_array_uint8_t
{
    _csal_attrib_t base;
};

struct _csal_attrib_array_uint16_t
{
    _csal_attrib_t base;
};

struct _csal_attrib_array_uint32_t
{
    _csal_attrib_t base;
};

struct _csal_attrib_array_uint64_t
{
    _csal_attrib_t base;
};

struct _csal_attrib_array_float32_t
{
    _csal_attrib_t base;
};

struct _csal_attrib_array_float64_t
{
    _csal_attrib_t base;
};

struct _csal_attrib_array_string_t
{
    _csal_attrib_t base;
};




#if 0
struct _csal_scalar_attrib_t
{
//    sal::object::Attribute* att;
};
struct _csal_string_attrib_t
{
};

#endif
//struct _csal_array_attrib_t
//{
//};
#if 0
struct _csal_attrib_branch_t
{
    _csal_attrib_t base;
};
#endif
#if 0
struct _csal_node_branch_t
{
	sal::node::Branch::Ptr sal_branch_ptr;
};
#endif


static CSAL_ATTRIBUTE_TYPE AttributeType_2_csal( sal::object::AttributeType sal_type )
{
    return (CSAL_ATTRIBUTE_TYPE)sal_type;
}

class csal_factory
{
    public:
        csal_attrib_t* create( CSAL_ATTRIBUTE_TYPE csal_type );
};


csal_attrib_t* csal_factory::create( CSAL_ATTRIBUTE_TYPE csal_type )
{
    csal_attrib_t* a = NULL;
    switch( csal_type )
    {
        case CSAL_ATTR_BOOL:
                break;
 
        case CSAL_ATTR_UINT8:
            a = (csal_attrib_t*)( csal_attrib_uint8_create( 0 ) );
                break;
        case CSAL_ATTR_UINT16:
            a = (csal_attrib_t*)( csal_attrib_uint16_create(0) );
                break;
        case CSAL_ATTR_UINT32:
            a = (csal_attrib_t*)( csal_attrib_uint32_create(0));
                break;
        case CSAL_ATTR_UINT64:
            a = (csal_attrib_t*)( csal_attrib_uint64_create(0));
                break;

        case CSAL_ATTR_INT8:
            a = (csal_attrib_t*)( csal_attrib_int8_create( 0 ) );
                break;
        case CSAL_ATTR_INT16:
            a = (csal_attrib_t*)( csal_attrib_int16_create(0) );
                break;
        case CSAL_ATTR_INT32:
            a = (csal_attrib_t*)( csal_attrib_int32_create(0));
                break;
        case CSAL_ATTR_INT64:
            a = (csal_attrib_t*)(csal_attrib_int64_create(0));
                break;

        case CSAL_ATTR_FLOAT32:
            a = (csal_attrib_t*)( csal_attrib_float_create(0.0));
                break;
        case CSAL_ATTR_FLOAT64:
            a = (csal_attrib_t*)(csal_attrib_double_create(0.0));
                break;

        case CSAL_ATTR_STRING:
            a = (csal_attrib_t*)(csal_attrib_string_create(""));
                break;
#if 0
        case CSAL_ATTR_BRANCH:
            a = (csal_attrib_t*)(csal_attrib_branch_create() );
                break;
#endif



#if 0
        case CSAL_ATTR_UINT8_ARRAY:
                break;
        case CSAL_ATTR_UINT16_ARRAY:
                break;
        case CSAL_ATTR_UINT32_ARRAY:
                break;
        case CSAL_ATTR_UINT64_ARRAY:
                break;
        case CSAL_ATTR_INT8_ARRAY:
                break;
        case CSAL_ATTR_INT16_ARRAY:
                break;
        case CSAL_ATTR_INT32_ARRAY:
                break;
        case CSAL_ATTR_INT64_ARRAY:
                break;

        case CSAL_ATTR_FLOAT32_ARRAY:
                break;
        case CSAL_ATTR_FLOAT64_ARRAY:
                break;

        case CSAL_ATTR_STRING_ARRAY:
                break;
#endif
        assert( a && "attribute does not have a factory" );




    }
    return a;
}

csal_attrib_t* csal_attrib_from_sal( sal::object::Attribute::Ptr sal_ptr )
{
    csal_factory fac;

    CSAL_ATTRIBUTE_TYPE csal_type =   AttributeType_2_csal( sal_ptr->type_id() );

    csal_attrib_t* csal_at_ptr = fac.create( csal_type );
    csal_at_ptr->sal_at_ptr = sal_ptr;

    return csal_at_ptr;
}

sal::object::Attribute::Ptr sal_attrib_from_csal( csal_attrib_t* csal_ptr )
{
    sal::object::Attribute::Ptr sal_ptr = csal_ptr->sal_at_ptr;
    return sal_ptr;
}



extern "C"
CSAL_ATTRIBUTE_TYPE csal_attrib_type( csal_attrib_t* self )
{
    sal::object::Attribute::Ptr at = self->sal_at_ptr.cast< sal::object::Attribute >();

    CSAL_ATTRIBUTE_TYPE csal_type = AttributeType_2_csal( at->type_id() );
    return csal_type;
}

extern "C"
csal_attrib_uint8_t* csal_attrib_uint8_create( uint8_t val )
{
    csal_attrib_uint8_t* self = new csal_attrib_uint8_t;
    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::UInt8( val ) );

    return self;
}


extern "C"
csal_attrib_int8_t* csal_attrib_int8_create( int8_t val )
{
    csal_attrib_int8_t* self = new csal_attrib_int8_t;
    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::Int8( val ) );
    return self;

}

extern "C"
int8_t csal_attrib_int8_value_get( csal_attrib_int8_t* self )
{
    sal::object::Int8::Ptr a = self->base.sal_at_ptr.cast< sal::object::Int8 >();
    return a->value();
}

extern "C"
uint8_t csal_attrib_uint8_value_get( csal_attrib_uint8_t* self )
{
    sal::object::UInt8::Ptr a = self->base.sal_at_ptr.cast< sal::object::UInt8 >();
    return a->value();
}




extern "C"
int16_t csal_attrib_int16_value_get( csal_attrib_int16_t* self )
{
    sal::object::Int16::Ptr a =self->base.sal_at_ptr.cast<sal::object::Int16>();
    return a->value();
}
extern "C"
int32_t csal_attrib_int32_value_get( csal_attrib_int32_t* self )
{
    sal::object::Int32::Ptr a = self->base.sal_at_ptr.cast< sal::object::Int32>();
    return a->value();
}
extern "C"
int64_t csal_attrib_int64_value_get( csal_attrib_int64_t* self )
{
    sal::object::Int64::Ptr a = self->base.sal_at_ptr.cast<sal::object::Int64>();
    return a->value();
}


extern "C"
float csal_attrib_float_value_get( csal_attrib_float_t* self )
{
    sal::object::Float32::Ptr a = self->base.sal_at_ptr.cast<sal::object::Float32>();
    return a->value();
}
extern "C"
double csal_attrib_double_value_get( csal_attrib_double_t* self )
{
    sal::object::Float64::Ptr a = self->base.sal_at_ptr.cast<sal::object::Float64>();
    return a->value();
}









extern "C"
uint16_t csal_attrib_uint16_value_get( csal_attrib_uint16_t* self )
{
    sal::object::UInt16::Ptr a = self->base.sal_at_ptr.cast<sal::object::UInt16>();
    return a->value();
}
extern "C"
uint32_t csal_attrib_uint32_value_get( csal_attrib_uint32_t* self )
{
    sal::object::UInt32::Ptr a = self->base.sal_at_ptr.cast< sal::object::UInt32 >();
    return a->value();
}
extern "C"
uint64_t csal_attrib_uint64_value_get( csal_attrib_uint64_t* self )
{
    sal::object::UInt64::Ptr a = self->base.sal_at_ptr.cast<sal::object::UInt64>();
    return a->value();
}











extern "C"
int csal_attrib_int8_value_set( csal_attrib_int8_t* self, int8_t val )
{
    sal::object::Int8::Ptr a = self->base.sal_at_ptr.cast<sal::object::Int8>();
    a->value() = val;
    return 0;
}
extern "C"
int csal_attrib_int16_value_set( csal_attrib_int16_t* self, int16_t val )
{
    sal::object::Int16::Ptr a = self->base.sal_at_ptr.cast<sal::object::Int16>();
    a->value() = val;

    return 0;
}
extern "C"
int csal_attrib_int32_value_set( csal_attrib_int32_t* self, int32_t val )
{
    sal::object::Int32::Ptr a = self->base.sal_at_ptr.cast< sal::object::Int32>();
    a->value() = val;

    return 0;
}
extern "C"
int csal_attrib_int64_value_set( csal_attrib_int64_t* self, int64_t val )
{
    sal::object::Int64::Ptr a = self->base.sal_at_ptr.cast<sal::object::Int64>();
    a->value() = val;

    return 0;
}


extern "C"
int csal_attrib_float_value_set( csal_attrib_float_t* self, float val )
{
    sal::object::Float32::Ptr a = self->base.sal_at_ptr.cast< sal::object::Float32 >();
    a->value() = val;

    return 0;
}
extern "C"
int csal_attrib_double_value_set( csal_attrib_double_t* self, double val )
{
    sal::object::Float64::Ptr a = self->base.sal_at_ptr.cast<sal::object::Float64>();
    a->value() = val;

    return 0;
}











extern "C"
int csal_attrib_uint8_value_set( csal_attrib_uint8_t* self, uint8_t val )
{
    sal::object::UInt8::Ptr a = self->base.sal_at_ptr.cast< sal::object::UInt8 >();
    a->value() = val;
    return 0;
}
extern "C"
int csal_attrib_uint16_value_set( csal_attrib_uint16_t* self, uint16_t val )
{
    sal::object::UInt16::Ptr a = self->base.sal_at_ptr.cast<sal::object::UInt16>();
    a->value() = val;

    return 0;
}
extern "C"
int csal_attrib_uint32_value_set( csal_attrib_uint32_t* self, uint32_t val )
{
    sal::object::UInt32::Ptr a = self->base.sal_at_ptr.cast<sal::object::UInt32>();
    a->value() = val;

    return 0;
}
extern "C"
int csal_attrib_uint64_value_set( csal_attrib_uint64_t* self, uint64_t val )
{
    sal::object::UInt64::Ptr a = self->base.sal_at_ptr.cast< sal::object::UInt64 >();
    a->value() = val;

    return 0;
}











extern "C"
csal_attrib_int16_t* csal_attrib_int16_create( int16_t val )
{
    csal_attrib_int16_t* self = new csal_attrib_int16_t;
    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::Int16( val ) );
    return self;
}

extern "C"
csal_attrib_int32_t* csal_attrib_int32_create( int32_t val )
{
    csal_attrib_int32_t* self = new csal_attrib_int32_t;
    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::Int32( val ) );
    return self;
}

extern "C"
csal_attrib_int64_t* csal_attrib_int64_create( int64_t val )
{
    csal_attrib_int64_t* self = new csal_attrib_int64_t;
    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::Int64( val ) );
    return self;
}

extern "C"
csal_attrib_float_t* csal_attrib_float_create( float val )
{
    csal_attrib_float_t* self = new csal_attrib_float_t;
    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::Float32( val ) );
    return self;
}

extern "C"
csal_attrib_double_t* csal_attrib_double_create( double val )
{
    csal_attrib_double_t* self = new csal_attrib_double_t;
    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::Float64( val ) );
    return self;
}








extern "C"
csal_attrib_uint16_t* csal_attrib_uint16_create( uint16_t val )
{
    csal_attrib_uint16_t* self = new csal_attrib_uint16_t;
    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::UInt16( val ) );
    return self;
}

extern "C"
csal_attrib_uint32_t* csal_attrib_uint32_create( uint32_t val )
{
    csal_attrib_uint32_t* self = new csal_attrib_uint32_t;
    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::UInt32( val ) );
    return self;
}

extern "C"
csal_attrib_uint64_t* csal_attrib_uint64_create( uint64_t val )
{
    csal_attrib_uint64_t* self = new csal_attrib_uint64_t;
    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::UInt64( val ) );
    return self;
}










extern "C"
csal_attrib_string_t* csal_attrib_string_create( const char* pszval )
{
    csal_attrib_string_t* self = new csal_attrib_string_t;
    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::String( pszval ) );
    return self;
}

extern "C"
int csal_attrib_destroy( csal_attrib_t *self )
{
    //use scope rules to ensure smart ptr destroyed before call to delete
    {
        sal::object::Attribute::Ptr tmp;
        self->sal_at_ptr.swap( tmp );
    }
    delete self;

    return 0;
}

#if 0
extern "C"
csal_node_branch_t* csal_node_branch_create( csal_node_nodeinfo_t* ni, const char* pszdesc )
{
    csal_node_branch_t* self = new csal_node_branch_t;

    self->sal_branch_ptr = sal::node::Branch::Ptr( new sal::node::Branch() );
    return self;
}
extern "C"
int csal_attrib_branch_set( csal_attrib_branch_t* self, const char* pszkey, csal_attrib_t* _at )
{
    int err = 0;

    sal::object::Attribute::Ptr sal_at_ptr = _at->sal_at_ptr;

    sal::object::Branch::Ptr sal_branch_ptr = self->base.sal_at_ptr.cast<sal::object::Branch>();

    sal_branch_ptr->set( pszkey, sal_at_ptr );

    return err;
}

extern "C"
csal_attrib_t* csal_attrib_branch_get( csal_attrib_branch_t* self, const char* pszkey )
{
    sal::object::Branch::Ptr branch = self->base.sal_at_ptr.cast<sal::object::Branch>();

    sal::object::Attribute::Ptr sal_at_ptr = branch->get( pszkey );

    csal_attrib_t* csal_ptr = csal_attrib_from_sal( sal_at_ptr );

    return csal_ptr;
}

extern "C"
csal_bool csal_attrib_branch_has( csal_attrib_branch_t* self, const char* pszkey )
{
    sal::object::Branch::Ptr branch = self->base.sal_at_ptr.cast<  sal::object::Branch >();
    std::string key( pszkey);

    csal_bool bhas = branch->has( key );
    return bhas;

}
extern "C"
int csal_attrib_branch_remove( csal_attrib_branch_t* self, const char* pszkey )
{
    sal::object::Branch::Ptr branch = self->base.sal_at_ptr.cast< sal::object::Branch >();
    std::string key( pszkey);

    branch->remove( key );
    return 0;

}
#endif

extern "C"
csal_attrib_array_float32_t* csal_attrib_array_float32_create( uint64_t* _shape, uint64_t nshapes )
{

    std::vector< uint64_t > shape( _shape, _shape+nshapes );

    csal_attrib_array_float32_t* self = new csal_attrib_array_float32_t;
    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::Float32Array( shape) );

    return self;
}

extern "C"
csal_attrib_array_float64_t* csal_attrib_array_float64_create( uint64_t* _shape, uint64_t nshapes )
{

    std::vector< uint64_t > shape( _shape, _shape+nshapes );

    csal_attrib_array_float64_t* self = new csal_attrib_array_float64_t;
    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::Float64Array( shape ) );

    return self;
}




extern "C"
csal_attrib_array_string_t* csal_attrib_array_string_create( uint64_t* _shape, uint64_t nshapes )
{
    std::vector< uint64_t > shape( _shape, _shape+nshapes );

    csal_attrib_array_string_t* self = new csal_attrib_array_string_t;

    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::StringArray( shape ) );

    return self;
}


extern "C"
csal_attrib_array_int8_t* csal_attrib_array_int8_create( uint64_t* _shape, uint64_t nshapes )
{
    std::vector< uint64_t > shape( _shape, _shape+nshapes );

    csal_attrib_array_int8_t* self = new csal_attrib_array_int8_t;

    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::Int8Array( shape ) );

    return self;
}

extern "C"
csal_attrib_array_int16_t* csal_attrib_array_int16_create( uint64_t* _shape, uint64_t nshapes )
{
    std::vector< uint64_t > shape( _shape, _shape+nshapes );

    csal_attrib_array_int16_t* self = new csal_attrib_array_int16_t;

    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::Int16Array( shape ) );

    return self;
}

extern "C"
csal_attrib_array_int32_t* csal_attrib_array_int32_create( uint64_t* _shape, uint64_t nshapes )
{
    std::vector< uint64_t > shape( _shape, _shape+nshapes );

    csal_attrib_array_int32_t* self = new csal_attrib_array_int32_t;

    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::Int32Array( shape ) );

    return self;
}

extern "C"
csal_attrib_array_int64_t* csal_attrib_array_int64_create( uint64_t* _shape, uint64_t nshapes )
{
    std::vector< uint64_t > shape( _shape, _shape+nshapes );

    csal_attrib_array_int64_t* self = new csal_attrib_array_int64_t;

    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::Int64Array( shape ) );

    return self;
}





extern "C"
csal_attrib_array_uint8_t* csal_attrib_array_uint8_create( uint64_t* _shape, uint64_t nshapes )
{
    std::vector< uint64_t > shape( _shape, _shape+nshapes );

    csal_attrib_array_uint8_t* self = new csal_attrib_array_uint8_t;

    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::UInt8Array( shape ) );

    return self;
}

extern "C"
csal_attrib_array_uint16_t* csal_attrib_array_uint16_create( uint64_t* _shape, uint64_t nshapes )
{
    std::vector< uint64_t > shape( _shape, _shape+nshapes );

    csal_attrib_array_uint16_t* self = new csal_attrib_array_uint16_t;

    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::UInt16Array( shape ) );

    return self;
}

extern "C"
csal_attrib_array_uint32_t* csal_attrib_array_uint32_create( uint64_t* _shape, uint64_t nshapes )
{
    std::vector< uint64_t > shape( _shape, _shape+nshapes );

    csal_attrib_array_uint32_t* self = new csal_attrib_array_uint32_t;

    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::UInt32Array( shape ) );

    return self;
}

extern "C"
csal_attrib_array_uint64_t* csal_attrib_array_uint64_create( uint64_t* _shape, uint64_t nshapes )
{
    std::vector< uint64_t > shape( _shape, _shape+nshapes );

    csal_attrib_array_uint64_t* self = new csal_attrib_array_uint64_t;

    self->base.sal_at_ptr = sal::object::Attribute::Ptr( new sal::object::UInt64Array( shape ) );

    return self;
}







extern "C"
const char* csal_attrib_array_string_element_get( csal_attrib_array_string_t* self, uint64_t* indices, uint64_t nindices )
{
    size_t i = 0;
    sal::object::StringArray::Ptr a = self->base.sal_at_ptr.cast<  sal::object::StringArray >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    const std::string& val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    return val.data();

}

extern "C"
float csal_attrib_array_float32_element_get( csal_attrib_array_float32_t* self, uint64_t* indices, uint64_t nindices )
{
    size_t i = 0;
    sal::object::Float32Array::Ptr a = self->base.sal_at_ptr.cast<  sal::object::Float32Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    float val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    return val;

}

extern "C"
double csal_attrib_array_float64_element_get( csal_attrib_array_float64_t* self, uint64_t* indices, uint64_t nindices )
{
    size_t i = 0;
    sal::object::Float64Array::Ptr a = self->base.sal_at_ptr.cast<  sal::object::Float64Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    double val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    return val;

}


extern "C"
int8_t csal_attrib_array_int8_element_get( csal_attrib_array_int8_t* self, uint64_t* indices, uint64_t nindices )
{
    size_t i = 0;
    sal::object::Int8Array::Ptr a = self->base.sal_at_ptr.cast<  sal::object::Int8Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    int8_t val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    return val;

}
extern "C"
int16_t csal_attrib_array_int16_element_get( csal_attrib_array_int16_t* self, uint64_t* indices, uint64_t nindices )
{
    size_t i = 0;
    sal::object::Int16Array::Ptr a = self->base.sal_at_ptr.cast<  sal::object::Int16Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    int16_t val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    return val;

}
extern "C"
int32_t csal_attrib_array_int32_element_get( csal_attrib_array_int32_t* self, uint64_t* indices, uint64_t nindices )
{
    size_t i = 0;
    sal::object::Int32Array::Ptr a = self->base.sal_at_ptr.cast<  sal::object::Int32Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    int32_t val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    return val;

}

extern "C"
int64_t csal_attrib_array_int64_element_get( csal_attrib_array_int64_t* self, uint64_t* indices, uint64_t nindices )
{
    size_t i = 0;
    sal::object::Int64Array::Ptr a = self->base.sal_at_ptr.cast<  sal::object::Int64Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    int64_t val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    return val;
}












extern "C"
uint8_t csal_attrib_array_uint8_element_get( csal_attrib_array_uint8_t* self, uint64_t* indices, uint64_t nindices )
{
    size_t i = 0;
    sal::object::UInt8Array::Ptr a = self->base.sal_at_ptr.cast<  sal::object::UInt8Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    uint8_t val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    return val;

}
extern "C"
uint16_t csal_attrib_array_uint16_element_get( csal_attrib_array_uint16_t* self, uint64_t* indices, uint64_t nindices )
{
    size_t i = 0;
    sal::object::UInt16Array::Ptr a = self->base.sal_at_ptr.cast<  sal::object::UInt16Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    uint16_t val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    return val;
}

extern "C"
uint32_t csal_attrib_array_uint32_element_get( csal_attrib_array_uint32_t* self, uint64_t* indices, uint64_t nindices )
{
    size_t i = 0;
    sal::object::UInt32Array::Ptr a = self->base.sal_at_ptr.cast<  sal::object::UInt32Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    uint32_t val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );

    return val;
}

extern "C"
uint64_t csal_attrib_array_uint64_element_get( csal_attrib_array_uint64_t* self, uint64_t* indices, uint64_t nindices )
{
    size_t i = 0;
    sal::object::UInt64Array::Ptr a = self->base.sal_at_ptr.cast<  sal::object::UInt64Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    uint64_t val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );

    return val;
}










extern "C"
int csal_attrib_array_string_element_set( csal_attrib_array_string_t* self, uint64_t* indices, uint64_t nindices, const char* val )
{
    size_t i = 0;
    sal::object::StringArray::Ptr a = self->base.sal_at_ptr.cast< sal::object::StringArray >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    std::string& el_val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    el_val = val;

    return 0;
}
extern "C"
int csal_attrib_array_float32_element_set( csal_attrib_array_float32_t* self, uint64_t* indices, uint64_t nindices, float val )
{
    size_t i = 0;
    sal::object::Float32Array::Ptr a = self->base.sal_at_ptr.cast< sal::object::Float32Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    float& el_val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    el_val = val;

    return 0;
}

extern "C"
int csal_attrib_array_float64_element_set( csal_attrib_array_float64_t* self, uint64_t* indices, uint64_t nindices, double val )
{
    size_t i = 0;
    sal::object::Float64Array::Ptr a = self->base.sal_at_ptr.cast< sal::object::Float64Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    double& el_val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    el_val = val;

    return 0;
}


extern "C"
int csal_attrib_array_int8_element_set( csal_attrib_array_int8_t* self, uint64_t* indices, uint64_t nindices, int8_t val )
{
    size_t i = 0;
    sal::object::Int8Array::Ptr a = self->base.sal_at_ptr.cast< sal::object::Int8Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    int8_t& el_val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    el_val = val;

    return 0;
}

extern "C"
int csal_attrib_array_int16_element_set( csal_attrib_array_int16_t* self, uint64_t* indices, uint64_t nindices, int16_t val )
{
    size_t i = 0;
    sal::object::Int16Array::Ptr a = self->base.sal_at_ptr.cast< sal::object::Int16Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    int16_t& el_val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    el_val = val;

    return 0;
}

extern "C"
int csal_attrib_array_int32_element_set( csal_attrib_array_int32_t* self, uint64_t* indices, uint64_t nindices, int32_t val )
{
    size_t i = 0;
    sal::object::Int32Array::Ptr a = self->base.sal_at_ptr.cast< sal::object::Int32Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    int32_t& el_val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    el_val = val;

    return 0;
}

extern "C"
int csal_attrib_array_int64_element_set( csal_attrib_array_int64_t* self, uint64_t* indices, uint64_t nindices, int64_t val )
{
    size_t i = 0;
    sal::object::Int64Array::Ptr a = self->base.sal_at_ptr.cast< sal::object::Int64Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    int64_t& el_val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    el_val = val;

    return 0;
}





extern "C"
int csal_attrib_array_uint8_element_set( csal_attrib_array_uint8_t* self, uint64_t* indices, uint64_t nindices, uint8_t val )
{
    size_t i = 0;
    sal::object::UInt8Array::Ptr a = self->base.sal_at_ptr.cast< sal::object::UInt8Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    uint8_t& el_val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    el_val = val;

    return 0;
}

extern "C"
int csal_attrib_array_uint16_element_set( csal_attrib_array_uint16_t* self, uint64_t* indices, uint64_t nindices, uint16_t val )
{
    size_t i = 0;
    sal::object::UInt16Array::Ptr a = self->base.sal_at_ptr.cast< sal::object::UInt16Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    uint16_t& el_val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    el_val = val;

    return 0;
}

extern "C"
int csal_attrib_array_uint32_element_set( csal_attrib_array_uint32_t* self, uint64_t* indices, uint64_t nindices, uint32_t val )
{
    size_t i = 0;
    sal::object::UInt32Array::Ptr a = self->base.sal_at_ptr.cast< sal::object::UInt32Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    uint32_t& el_val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    el_val = val;

    return 0;
}

extern "C"
int csal_attrib_array_uint64_element_set( csal_attrib_array_uint64_t* self, uint64_t* indices, uint64_t nindices, uint64_t val )
{
    size_t i = 0;
    sal::object::UInt64Array::Ptr a = self->base.sal_at_ptr.cast< sal::object::UInt64Array >();

    int64_t idxs[10];

    for( i = 0; i < nindices; ++i )
        idxs[i] = indices[i];

    for( i=nindices; i < 10; ++i )
        idxs[i]=-1;

    uint64_t& el_val = a->at( idxs[0], idxs[1], idxs[2], idxs[3], idxs[4], idxs[5], idxs[6], idxs[7], idxs[8], idxs[9] );
    el_val = val;

    return 0;
}


#if 0
const char* csal_attrib_array_eltype( csal_attrib_array_t* self )
{
    sal::object::Array* arr = (sal::object::Array*)self;
    const std::string& str = arr.element_type;
    return str.c_str();
}
#endif






/* put in casl_attrib when global variables are removed from headers */
struct _csal_node_object_t
{
    sal::node::NodeObject::Ptr sal_nodeobj_ptr;
};

struct _csal_client_t
{
	sal::Client* sal_client;
};

int csal_client_create( csal_client_t** pself, const char* pszhost, csal_bool_t verify_https_cert )
{
	int err = 0;
	*pself = new csal_client_t;

	try
	{
		(*pself)->sal_client = new sal::Client( pszhost, verify_https_cert );
	}
	catch( sal::exception::SALException& ex )
	{
		/* TODO : convert exception to error */
		err=1;
	}

	return err;
}

int csal_client_destroy( csal_client_t* self )
{
	int err=0;

	delete self->sal_client;
	delete self;

	return err;
}

int csal_client_host_get( csal_client_t* self, char* buf, size_t szbuf )
{
	int err = 0;

	std::string host = self->sal_client->get_host();

	strncpy( buf, host.data(), szbuf-1 );
	buf[ szbuf-1 ] = '\0';

	return err;
}

int csal_client_host_set( csal_client_t* self, char* pszhost )
{
	int err = 0;

	try
	{
		self->sal_client->set_host( pszhost );
	}
	catch( sal::exception::SALException& ex )
	{
		/* TODO: convert exception */
		err=1;
	}

	return err;
}

csal_bool_t csal_client_is_auth_required( csal_client_t* self )
{
	return (self->sal_client->is_auth_required() ? csal_true : csal_false);
}


int csal_client_authenticate( csal_client_t* self, const char* pszuser, const char* pszpasswd )
{
	int err = 0;
	return err;
}

int csal_client_attrib_get( csal_client_t* self, const char* pszpath, csal_bool_t summary )
{
	int err = 0;

	sal::object::Attribute::Ptr sal_at = self->sal_client->get( pszpath, summary );

	return err;
}


struct _csal_node_leaftype_t
{
};
#if 0
struct _csal_node_report_t
{
	sal::node::Report::Ptr ptr;
};
#endif
#if 0
struct _csal_node_branch_t
{
#if 0
    csal_node_object_t base;
#endif
};
#endif

#if 0
struct _csal_node_leaf_t
{
    _csal_node_object_t base;
};
#endif

#if 0
extern "C"
CSAL_NODE_TYPE csal_node_object_type( csal_node_object_t* self )
{
   sal::node::Object::Ptr sal_obj_ptr = self->sal_obj_ptr;
    //sal::node::NodeType sal_type = sal_obj_ptr->type;
    
    assert( csal_false && "Not implemented" );

    return CSAL_NODE_TYPE_BRANCH;
}
#endif

#if 0
extern "C"
csal_node_branch_t* csal_node_branch_create( const char* pszdescription )
{
    csal_node_branch_t* csal_ptr = new csal_node_branch_t;
    csal_ptr->base.sal_obj_ptr = sal::node::Branch::Ptr( new sal::node::Branch( pszdescription ) );

    return csal_ptr;
}

extern "C"
csal_node_leaf_t* csal_node_leaf_create( const char* pszcls
                                                , const char* pszgroup
                                                , uint64_t version
                                                , csal_bool summary
                                                , const char* pszdescription )
{
    csal_node_leaf_t* csal_ptr = new csal_node_leaf_t;
    csal_ptr->base.sal_obj_ptr = sal::node::Leaf::Ptr( new sal::node::Leaf( pszcls, pszgroup, version, summary, pszdescription ) );

    return csal_ptr;
}
#if 0
extern "C"
csal_attrib_t* csal_node_leaf_get( csal_node_leaf_t* self, const char* pszkey )
{
    sal::node::Object::Ptr sal_obj_ptr = self->base.sal_obj_ptr;
    //sal::node::Leaf::Ptr sal_leaf_ptr = sal_obj_ptr.cast< sal::node::Leaf >();
    //sal::object::Attribute::Ptr sal_ptr = sal_leaf_ptr.get( pszkey );
    
    //csal_attrib_t* csal_ptr = csal_attrib_from_sal( sal_ptr );

    return NULL/*csal_ptr*/;
}

extern "C"
int csal_node_object_destroy( csal_node_object_t* self )
{
    {
        sal::node::Object::Ptr tmp;
        tmp.swap( self->sal_obj_ptr);
    }
    delete self;

    return 0;
}
#endif
#endif

csal_node_object_t* csal_node_object_create( csal_node_info_t* node_info, const char* pszdescription, CSAL_NODE_TYPE node_type )
{
	csal_node_object_t* p = new csal_node_object_t;
	std::string description = pszdescription;
	//sal::node::NodeInfo sal_node_info;
	//p->sal_nodeobj_ptr = sal::node::NodeObject::Ptr( new sal::node::NodeObject( sal_node_info, description, (sal::node::NodeType)node_type ) );
}
int csal_node_object_destroy( csal_node_object_t* self )
{
    {
        sal::node::NodeObject::Ptr tmp;
        tmp.swap( self->sal_nodeobj_ptr);
    }
    delete self;

    return 0;
}

csal_bool_t csal_node_object_isleaf( csal_node_object_t* self )
{
	return self->sal_nodeobj_ptr->is_leaf() ? csal_true : csal_false;
}

csal_bool_t csal_node_object_isbranch( csal_node_object_t* self )
{
	return self->sal_nodeobj_ptr->is_branch() ? csal_true : csal_false;
}


struct _csal_node_info_t
{
	sal::node::NodeInfo* sal_node_info_ptr;
};

csal_node_info_t* csal_node_info_create( const char* pszcls, const char* pszgroup, int version )
{
	csal_node_info_t* self = new csal_node_info_t;
	self->sal_node_info_ptr = new sal::node::NodeInfo( pszcls, pszgroup, version );
	return self;
}

int csal_node_info_destroy( csal_node_info_t* self )
{
	int err = 0;

	delete self->sal_node_info_ptr;
	delete self;

	return err;
}
