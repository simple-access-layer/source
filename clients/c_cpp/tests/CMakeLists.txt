# catch.h is a single file header-only lib, no need to link any lib
#file(GLOB TEST_SOURCES test*.cpp)

add_executable(sal_test_attributes "test_attributes.cpp")

target_link_libraries(sal_test_attributes
PUBLIC
    Poco::Foundation
    Poco::JSON
    Poco::Net
    Poco::NetSSL
)