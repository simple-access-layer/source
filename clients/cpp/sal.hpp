#ifndef __SAL_H__
#define __SAL_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//struct sal_object {
//    sal_object_type_t type,
//    sal_object_data_t *object
//}
//
//enum sal_report_class(LEAF_REPORT, BRANCH_REPORT)
//typedef enum sal_report_class sal_report_class_t
//
//enum sal_data_object(FULL_OBJECT, SUMMARY_OBJECT)
//typedef enum sal_report_class sal_report_class_t


#define VALID_NODE_CHARS = "abcdefghijklmnopqrstuvwxyz0123456789-_."

/*
data object types to represent:

Atomic

Signed integer: int8, int16, int32, int64
Unsigned integer: uint8, uint16, uint32, uint64
Floating point: fp32, fp64
Boolean: bool
String: string
Strings are encoded as UTF8.

handle UTF8 strings as pure byte arrays

Container

N dimensional array: array<TYPE>
Where array type is any one of the atomic types.

Branch: branch / {}
The branch type is equivalent to a Python dictionary with string keys.

Special

null
When an attribute is labelled as optional, null is used inplace of missing content.
*/

/*
sal objects
-----------

reports:
branch, leaf

data (includes branches):
full, summary
*/


struct sal_server;

struct sal_report;
struct sal_leaf_report;
struct sal_branch_report;

struct sal_dataobject;

int sal_connect(char* host_url, sal_server *server)
int sal_authenticate(sal_server *server, char *username, char *password)
int sal_list(sal_server host, sal_report *obj, char *path, int summary)
int sal_get(sal_server host, char *path, int summary, sal_data *obj)
int sal_put(sal_server host, char *path, sal_data *obj)
int sal_delete(sal_server host, char *path)
int sal_copy(sal_server host, char *src_path, char *dest_path)

bool sal_is_int32(sal_variable *v)
void sal_pack_int32(sal_variable *v, int32_t *i)
int32_t sal_unpack_int32(sal_variable *v)




##endif
