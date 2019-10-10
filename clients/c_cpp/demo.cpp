#include "sal.h"
#include <stdint.h>
#include <float.h>
//#include <stdarg.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Exception.h"
#include <initializer_list>
#include <array>
//
//void get(std::initializer_list<int> list)
//{
//    std::cout << "arg count: " << list.size() << endl;
//    for(auto elem: list )
//    {
//        std::cout << elem << std::endl ;
//    }
//}


#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/JSON/Parser.h"
#include "Poco/URI.h"
#include "Poco/StreamCopier.h"

using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::URI;
using Poco::StreamCopier;



using namespace std;
using namespace sal::object;

int main(int argc, char **argv) {

    sal::Client sal("https://sal.jet.uk");
    cout << sal.get_host() << endl;


    sal.get("/data/pulse/latest?object=full");
    sal.get("/data/pulse/87738/ppf/signal/jetppf/magn/ipla?object=summary");
//    sal.get("/data/pulse/87738/ppf/signal/jetppf/magn/ipla?object=full");




//    sal.verify_https_cert = false;
//    sal.set_host("https://sal-dev.jet.uk/");


    exit(0);

//
//
//
//
//    // connect and setup request
//    URI uri("https://sal-dev.jet.uk/data/pulse/87737/ppf/signal/jetppf/magn/ipla?object=full");
////    URI uri("https://sal.jet.uk/data/pulse/latest?object=full");
//    cout << "SCHEME: " << uri.getScheme() << endl;
//    HTTPSClientSession session(uri.getHost(), uri.getPort());
//    HTTPRequest request(HTTPRequest::HTTP_GET, uri.getPathEtc(), HTTPMessage::HTTP_1_1);
//    HTTPResponse response;
//
//    // print request
////    request.write(std::cout);
//
//    // make request
//    session.sendRequest(request);
//
//    // convert response to a string
//    string json;
//    StreamCopier::copyToString(session.receiveResponse(response), json);
//    cout << json << endl;
//
//    if (response.getStatus() != Poco::Net::HTTPResponse::HTTP_OK) throw sal::exception::SALException();
//
//    Poco::JSON::Parser parser;
//    Poco::Dynamic::Var decoded = parser.parse(json);
//    Poco::JSON::Object::Ptr obj = decoded.extract<Poco::JSON::Object::Ptr>();
//
//    Poco::JSON::Object::Ptr content = obj->getObject("object");
//    content->stringify(cout, 2);
//    cout << endl;
//
//    sal::node::Leaf::Ptr leaf = sal::node::Leaf::decode(content);
//
//    cout << leaf->cls << endl;
//    cout << leaf->has("description") << endl;
//    cout << leaf->get_as<String>("description")->value << endl;
////    cout << leaf->get_as<UInt64>("value")->value << endl;
//    cout << leaf->get("description").cast<String>()->value << endl;
////    cout << leaf->get_as<Branch>("dimensions")->get_as<Branch>("0")->get_as<String>("description")->value << endl;



// -------------------JUNK

//    sal::StringArray sa;
//    sa.data.

//
//    sal::Client client("https://sal.jet.uk");
//    sal::Report rpt = client.list("/pulse/87737/ppf/signal/jetppf/magn/ipla"); // USE c++11 move symantics
//    sal::Object obj = client.get("/pulse/87737/ppf/signal/jetppf/magn/ipla", false);  // USE c++11 move symantics
//
//    if (rpt.is_leaf()) {
//        // blah
//    } else if {
//        // blah
//    }
//
//    cout << "Object type: " << obj.type() << endl;
//
//    int n;
//    cin >> n;
//
//    for (int j=0;j<n;j++) {
//        get({j,2,3,4,5});
//    }

}