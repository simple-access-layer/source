
#include <float.h>
#include <stdint.h>
//#include <stdarg.h>
#include <array>
#include <initializer_list>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Poco/Dynamic/Var.h"
#include "Poco/Exception.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"

#include "sal.h"

using Poco::StreamCopier;
using Poco::URI;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPSClientSession;


using namespace sal::object;

// todo:  this demo should follow sal python tutorial as possible
// url:

int main(int argc, char** argvstring)
{

    sal::Client sal("https://sal.jet.uk");
    // could be also dev test server to test data modification on server
    // sal::Client sal("http://cnlpepper.net");

    std::cout << "SAL host: " << sal.get_host() << endl;
    std::cout << "Is auth required? " << (sal.is_auth_required() ? "Yes" : "No") << std::endl;

    sal.authenticate("lkjlkj", "lkjlkj"); // todo: not yet done on server side?

    auto j = sal.get("pulse/latest"); // by default, get the full object
    // what if get url is branch, not leaf?
    if (!j) // if any error happened, nullptr will be returned
    {
        return EXIT_FAILURE;
    }
    // parse an uint64 pulse number,  decode_scalar()

    //    sal.verify_https_cert = false;
    //    sal.set_host("https://sal-dev.jet.uk/");

    // demo sal.list(branch_path)
    // auto b = sal.get("pulse/87738/ppf/signal/jetppf/magn")

    auto s = sal.get("pulse/87738/ppf/signal/jetppf/magn/ipla?object=summary");
    //    sal.get("/data/pulse/87738/ppf/signal/jetppf/magn/ipla?object=full");
    // https://sal.jet.uk/data/pulse/latest?object=full

    // get a full object, and decode as a general DataObject (key-attribute dict)

    // is there any way to detect the template parameter? float or double
    // get the full object , decode as Signal<float>

    // STL iterator and algorithm demo on Array<T> max()

    // optional: using Array<T> as EigenMatrix

    exit(0);
}