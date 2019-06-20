#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "Poco/Exception.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"

using namespace std;

int main(int argc, char**argv) {

    stringstream ss;
    string in = "1238946368726863j;ljfl;jljf;lakjf;lj;ljasdklfjjiu89uoijanaf;dsnlknklaj;lfj;ljs;flj;ljdsf;ljlasdfj4kjbvjbhjsbjbmjs%&^(^&*(^)";
    string decoded;

    Poco::Base64Encoder encoder(ss, Poco::BASE64_URL_ENCODING);
    encoder << in;
    encoder.close();

    cout << ss.str() << endl;

    Poco::Base64Decoder decoder(ss);

    decoder >> decoded;
    cout << decoded << endl;
    cout << in << endl;





}


