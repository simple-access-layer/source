#ifndef __CSAL_CLIENT_H__
#define __CSAL_CLIENT_H__

#include "csal/csal.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct _csal_client_t csal_client_t;


	int csal_client_create( csal_client_t** pself, const char* pszhost, csal_bool_t verify_https_cert );
	int csal_client_destroy( csal_client_t* self );

	csal_bool_t csal_client_is_auth_required( csal_client_t* self );

	int csal_client_authenticate( csal_client_t* self, const char* pszuser, const char* pszpasswd );

#if 0
	csal_report_t* csal_client_report( csal_client_t* self );
#endif
	int csal_client_put( csal_client_t* self, const char* pszpath, const csal_object_t* obj );
	int csal_client_get( csal_client_t* self, const char* pszpath, csal_bool_t summary );
	int csal_client_copy( csal_client_t* self, const char* psztarget, const char* pszsource );
	int csal_client_del( csal_client_t* self, const char* pszpath );

#ifdef __cplusplus
}
#endif


#endif
