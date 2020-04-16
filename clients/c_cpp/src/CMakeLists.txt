# this subfolder CMakeLists.txt compile sal.cpp to libsal.so

add_library(sal "sal.cpp")

target_link_libraries(sal
PUBLIC     ${Poco_LIBRARIES}
)