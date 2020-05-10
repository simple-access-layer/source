
# openssl-dev must be installed, but not needed to find the package
#find_package(OpenSSL REQUIRED)

if(Poco_FOUND)  # 
    add_executable(sal_client_example sal_https_client.cpp)
    target_link_libraries(sal_client_example 
        PUBLIC ${Poco_LIBRARIES}
    )
    target_compile_options(sal_client_example PRIVATE -Wall -Wextra -Wno-unused)

    add_executable(sal_demo demo.cpp)
    target_link_libraries(sal_demo
        PUBLIC ${Poco_LIBRARIES}
    )
    target_compile_options(sal_demo PRIVATE -Wall -Wextra -Wno-unused)
else()
    message(FATAL_ERROR "Poco components is not found")
endif()