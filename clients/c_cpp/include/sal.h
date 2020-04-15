#ifndef __SAL_H__
#define __SAL_H__


// todo: proper exception handling, current catch all in various places is just a quick kludge

#include "sal_data.h" /// depends on Poco::JSON
// sal::node namespace in sal_core.h depends on sal_data.h
#include "sal_core.h"

#include "sal_client.h" /// depends on all headers above, and also Poco::Net

#endif
