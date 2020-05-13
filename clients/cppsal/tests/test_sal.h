#pragma once
// this header only is used for internal unit test only, not a public API

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/UTF8String.h"

/// there is no auth, so it will only work within intranet
Poco::JSON::Object::Ptr get_json(const std::string url)
{
    using namespace Poco;
    using namespace Poco::Net;
    using namespace Poco::JSON;

    HTTPResponse response;
    Poco::JSON::Parser parser;
    std::string json;
    Poco::URI uri{url};

    // https session, ignoring invalid certificates
    auto session = new HTTPSClientSession(uri.getHost(), uri.getPort(),
                                          new Context(Context::CLIENT_USE, "", Context::VERIFY_NONE));

    HTTPRequest request(HTTPRequest::HTTP_GET, url, HTTPMessage::HTTP_1_1);
    session->sendRequest(request);
    StreamCopier::copyToString(session->receiveResponse(response), json);
    // cout << "res.getStatus() = " << response.getStatus() << std::endl;

    // todo: handle exceptions and check response status
    auto j = parser.parse(json).extract<Poco::JSON::Object::Ptr>(); // decode json
    // j->stringify(cout, 2);    // debugging output
    return j;
}