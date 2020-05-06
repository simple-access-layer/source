## Installation

Poco and Boost are the only lib needs to compile.  Poco itself depends on openssl development package.

git cmake 3, doxygen

### Install dependencies on Debian/Ubuntu
On Ubuntu 18.04 version is Poco5 v1.8
`sudo apt-get install libpoco-dev` for networking
`sudo apt-get install libboost-dev` for config file parsing, which may be replaced by regex matching. Or python client, using JSON as config file format.

POCO is a networking framework it contains JSON, format, timestamp. 

### Install dependencies on Fedora and Centos

`centos/devtoolset-7-toolchain-centos7` has g++ compiler, make,  etc.
local test interactively: 
`docker run -it --volume "${PWD}:/mnt/sal" --user root --workdir /mnt/sal centos/devtoolset-7-toolchain-centos7 bash`

Freia has RHEL 7,  poco-devel-1.6.1-3.el7.x86_64.rpm
`yum install epel-release -y && yum install poco-devel openssl-devel cmake3 -y`

cmake version is a bit too low 2.8, POCO C++ Libraries requires CMake 3.2 or higher.

but cmake3 can be installed from EPEL, alias cmake3 to cmake

g++ 7.3

### Build Poco from source

see Poco project guideline, on [building using cmake](https://pocoproject.org/docs/00200-GettingStarted.html)

Once compiled, export the env var `Poco_DIR`, then FindPoco.cmake should be able to detect poco. 

### Other optional Dependency

C++ library corresponding to Python ConfigParser

1. `boost::property_tree` supports xml, json and inf files

```cpp
//<boost/property_tree.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

...

boost::property_tree::ptree pt;
boost::property_tree::ini_parser::read_ini("config.ini", pt);
std::cout << pt.get<std::string>("Section1.Value1") << std::endl;
std::cout << pt.get<std::string>("Section1.Value2") << std::endl;
```

2. other header-only choices
+ inih: 
+ inicpp

https://github.com/weisslj/cpp-argparse
boost::program_options,  variable_map (std::map<std::string, variable>), variable is  based on `std::any`

3. Eigen for ND array
 xtensor


+ json parsing: nulhmann::json, Poco has Json module
```
using json = nlohmann::json;

json parsed_data = json::parse(input);
```

+ format: `libfmt-dev` or Poco formatting
`sudo apt-get install libfmt-dev`

Yes, you need to define FMT_HEADER_ONLY before including any fmt's code.
```
#define FMT_HEADER_ONLY
#include "fmt/format.h"
```
cmake "find_package("fmt")

### Release
PackPack works best with GitHub, Travis CI and PackageCloud:

Push your code to GitHub

Build packages using Travis CI
Friea OS: `el7.x86_64`,  

Host repositories on PackageCloud