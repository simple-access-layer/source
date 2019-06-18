#include <iostream>
#include <sstream>
#include <vector>
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Exception.h"

using namespace std;


class JSONException : public exception {
    const char* what() const noexcept {
        return "An error occured while attempting to serialise/deserialise a JSON object.";
    }
};

Poco::JSON::Object::Ptr deserialise_json(const string json) {

    Poco::JSON::Parser parser;

    try {
        Poco::Dynamic::Var decoded = parser.parse(json);
        return decoded.extract<Poco::JSON::Object::Ptr>();
    } catch(Poco::Exception e) {
        throw JSONException();
    }
}


string serialise_json(const Poco::JSON::Object::Ptr obj) {
    stringstream s;
    obj->stringify(s);
    return s.str();
}


int main(int argc, char**argv) {

    Poco::JSON::Object::Ptr obj;

    obj = deserialise_json("{\"_class\": \"signal\", \"_group\": \"signal\", \"_version\": { \"type\": \"uint64\", \"value\": 1}}");

    obj->set("units", "Amps");
    obj->set("data", vector<int> {1,2,3,4,5});

    string json = serialise_json(obj);
    cout << json << endl;


    // create json object
    //root = new Object();
    //root->




//    root->stringify(cout);
//    cout << endl;

    obj = deserialise_json("I AM BAD");

}


