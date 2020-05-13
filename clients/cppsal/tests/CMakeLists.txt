# catch.h is a single file header-only lib, no need to link any lib
#file(GLOB TEST_SOURCES test*.cpp)

add_executable(sal_test_attributes "test_attributes.cpp")

target_link_libraries(sal_test_attributes
    PUBLIC     ${Poco_LIBRARIES}
        #        Poco::NetSSL
        #        Poco::Foundation
        #        Poco::Net
        #        Poco::JSON
)

add_executable(sal_test_nodeobjects "test_nodeobjects.cpp")

target_link_libraries(sal_test_nodeobjects
    PUBLIC     ${Poco_LIBRARIES}
)

add_executable(sal_data_object "test_data_object.cpp")

target_link_libraries(sal_data_object
    PUBLIC     ${Poco_LIBRARIES}
)