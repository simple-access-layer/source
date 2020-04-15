
find_package(OpenSSL REQUIRED)

if(TRUE)  # POCO_FOUND variable is not defiend even package is found
    message(STATUS "found Poco components")
    add_executable(sal_client_example sal_https_client.cpp)
    target_link_libraries(sal_client_example 
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