# On Ubuntu, there is a bug to locate the cpprestsdk-config.cmake file
# /usr/lib/x86_64-linux-gnu/cmake/cpprestsdk-config.cmake # install by apt
# pass options to cmake
# `cmake .. -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake`
# instead of set variable here
# set ( CMAKE_PREFIX_PATH /usr/local/lib/cmake/ )  # compiled from source

find_package(OpenSSL REQUIRED)

find_package(cpprestsdk)
if(FALSE)
    find_package(Boost COMPONENTS system REQUIRED)
    # compiler linkage options: -lcrypto -lssl -lcpprest -lboost_system
    add_executable(cpprest_example bingrequest.cpp)
    target_link_libraries(cpprest_example Boost::system)
    target_link_libraries(cpprest_example crypto)
    target_link_libraries(cpprest_example ssl)
    target_link_libraries(cpprest_example cpprestsdk::cpprest) # libcpprest.so
endif()

if(TRUE)  # POCO_FOUND variable is not defiend even package is found
    message(STATUS "found Poco components")
    add_executable(poco_client_example poco_https_client.cpp)
    target_link_libraries(poco_client_example 
        PUBLIC
            Poco::NetSSL
            Poco::Foundation
            Poco::Net
            Poco::JSON
    )

    add_executable(sal_demo demo.cpp)
    target_link_libraries(sal_demo
        PUBLIC
            Poco::NetSSL
            Poco::Foundation
            Poco::Net
            Poco::JSON
    )
else()
    message(FATAL_ERROR "Poco components is not found")
endif()