cmake_minimum_required(VERSION 3.7)
#cmake_minimum_required(VERSION 2.8.11)  # centos7, not tested yet

project(sal_cpp)
set(PROJECT_BRIEF "Simple access layer (SAL) in C++, by Qingfeng Xia, UKAEA, 2020")

set(PACKAGE_VERSION_MAJOR "0")
set(PACKAGE_VERSION_MINOR "3")
set(PACKAGE_VERSION_PATCH "0")

execute_process(
    COMMAND
        git rev-parse --short HEAD
    RESULT_VARIABLE
        SHORT_HASH_RESULT
    OUTPUT_VARIABLE
        PACKAGE_VERSION_COMMIT_HASH)


# this version name is used by CPack
set(PACKAGE_VERSION_NAME ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR})

set(PROJECT_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR})

##############################################################
# to user other compiler,  `CXX=clang++  cmake ..`
# std can and should be applied to target only
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# this seems only for GCC and CLANG compiler
# if((${CMAKE_CXX_STANDARD} GREATER 17)  OR  (${CMAKE_CXX_STANDARD} EQUAL 17))
#     link_libraries(stdc++fs)
# else()
#     find_package(Boost REQUIRED COMPONENTS
#                 filesystem)
#     link_libraries(Boost::filesystem)
# endif()

#################################################################
# options are cached, `rm -rf build` dir and start freshly
###################### module selection ##########################

option(SAL_USE_TYPE "use type system in base modlule" ON)
option(SAL_USE_EIGEN "use Eigen represent multi-dimension array" OFF)
option(SAL_USE_TEST "use unit test module" ON)

####################################################

# extra search path provided by this project, such OCCT SMesh
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cMake")


# it can be used by subdirectory cmakelist.txt
include_directories("./include")
# for json, must be run before include App subfolder
include_directories("${PROJECT_SOURCE_DIR}/third-party") 


## put all targets in bin and lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})


find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

##########################################################
# compulsary dependency
find_package(Poco REQUIRED COMPONENTS Foundation Util Net NetSSL Crypto JSON)
include_directories(${Poco_INCLUDE_DIRS})

if (UNIX)
    include("${PROJECT_SOURCE_DIR}/cMake/IdentifyOS.cmake")
    message("OS detected by `IdentifyOS.cmake` as `${CMAKE_OS_NAME}`")
endif()

if (UNIX)
    if (NOT DEFINED Poco_LIBRARIES})     # use FindPoco.cmake provided in this repo
        message(STATUS "Poco_LIBRARIES = ${Poco_LIBRARIES}")
    else()    # system provided Poco::JSON, to make it compatible with Poco_LIBRARIES
        set(Poco_LIBRARIES "-lPocoNet -lPocoNetSSL -lPocoFoundation -lPocoJSON")
        message(STATUS "Poco_LIBRARIES = ${Poco_LIBRARIES}")
    endif()
endif()


if(SAL_USE_EIGEN)
    find_package(Eigen3 REQUIRED NO_MODULE)
    if(EIGEN3_FOUND)
        message("found Eigen version: ${EIGEN3_VERSION}")
        include_directories(${EIGEN3_INCLUDE_DIRS})  # already on system include path
        #target_link_directories()  # or it is header only lib
    else()
        ## may download to third-party from git
    endif()
endif()

############################################################

#add_subdirectory("core")
#add_subdirectory("dataclass")

#add_subdirectory("src")
add_subdirectory("examples")

# deal with data and script subdirectory
#if(NOT EXISTS "${PROJECT_SOURCE_DIR}/third-party/loguru")
#message(FATAL_ERROR "have you forgot to download all other submodule before run cmake\n"
#        "in the repo root folder, run `git submodule update --init --recursive`")
#endif()


############################################################
# unit test module
############################################################
if(SAL_USE_TEST)
    add_subdirectory(tests)  # disable it since there is compiling error
    #add_subdirectory(third-party/Catch2)
endif()


###########################################
# Check 32/64 bit platform #
# copy from FreeCAD project, LGPL v3
###########################################
if (${CMAKE_SIZEOF_VOID_P} MATCHES "8") # It is 64bit, otherwise 32 bit systems match 4
	add_definitions(-D_OCC64)
	set(BIT_LENGTH 64)
else (${CMAKE_SIZEOF_VOID_P} MATCHES "8")
	set(BIT_LENGTH 32)
endif(${CMAKE_SIZEOF_VOID_P} MATCHES "8")

# Define helper macro option_with_default
macro( option_with_default OPTION_NAME OPTION_STRING OPTION_DEFAULT )
	if( NOT DEFINED ${OPTION_NAME} )
		set( ${OPTION_NAME} ${OPTION_DEFAULT} )
	endif( NOT DEFINED ${OPTION_NAME} )
	option( ${OPTION_NAME} "${OPTION_STRING}" ${${OPTION_NAME}} )
endmacro( option_with_default OPTION_NAME OPTION_STRING OPTION_DEFAULT )

############################################################
# Create DEB, RPM, activated by the command `make package` or `cpack`
# simultaneously building multiple package types -DCPACK_GENERATOR="DEB;RPM")
# conda, pip 
############################################################

if (UNIX)
    #include("${PROJECT_SOURCE_DIR}/cMake/IdentifyOS.cmake")
    if (${CMAKE_OS_NAME} STREQUAL "Debian" OR ${CMAKE_OS_NAME} STREQUAL "Ubuntu")
        # Tell CPack to generate a .deb package
        set(CPACK_GENERATOR "DEB")
    endif()
    if (${CMAKE_OS_NAME} STREQUAL "RedHat" OR ${CMAKE_OS_NAME} STREQUAL "Fedora")
        set(CPACK_GENERATOR "RPM")
    endif()
    if (${CMAKE_OS_NAME} STREQUAL "OSX")
        message("OSX packaging is not yet setup")
        #set(CPACK_GENERATOR "RPM")
    endif()
endif()

set(CPACK_PACKAGE_NAME parallel-preprocessor )
# Set a Package Maintainer. This is required
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Qingfeng Xia @ UKAEA")
#set(CPACK_PACKAGE_DESCRIPTION, ${PROJECT_BRIEF})
set(CPACK_PACKAGE_DESCRIPTION, "${CMAKE_CURRENT_SOURCE_DIR}/Readme.md")
# Set a Package Version
set(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION_NAME})
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")

#################################################################
#  split into the runtime, development component packages
# COMPONENT and GROUP are different concepts
# https://gitlab.kitware.com/cmake/community/-/wikis/doc/cpack/Component-Install-With-CPack
#################################################################

set(SAL_SINGLE_PACKAGE ON)
set(SAL_PACKAGE_PROVIDES "salcpp")
if(NOT SAL_SINGLE_PACKAGE)
    set(CPACK_COMPONENTS_ALL applications libraries headers data)
    # all components in one package is the default behaviour
    # windows and macos may select component in the GUI installer wizard
    # otherwise generate multple component packages on Linux
    set(CPACK_RPM_COMPONENT_INSTALL ON)
    set(CPACK_DEB_COMPONENT_INSTALL ON)
endif()

# set(CPACK_COMPONENTS_GROUPING IGNORE)  #1 package per component
# set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE) # 1 package for all 
# 1 package per component group the default behavior.
#set(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")
#set(CPACK_COMPONENT_LIBRARIES_GROUP "Development")
#set(CPACK_COMPONENT_HEADERS_GROUP "Development")


#####################################################################
# Debian-specific CPack variables                  
# to enable, run cmake with -DCPACK_GENERATOR="DEB", or uncomment       
#set(CPACK_GENERATOR "DEB")                                            
#######################################################################
if( CPACK_GENERATOR MATCHES ".*DEB.*" )
	set(CPACK_DEBIAN_PACKAGE_SECTION "science")
	# potentially split nonGui and Gui packages, lots of dep are given by OCC
	set(CPACK_DEBIAN_PACKAGE_BUILDS_DEPENDS "cmake (>= 2.8), librestcpp-dev")
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3), libstdc++6 (>= 4.4.0)")
	set(CPACK_DEBIAN_PACKAGE_SUGGESTS "freecad")
	set(CPACK_DEBIAN_PACKAGE_PROVIDES ${SAL_PACKAGE_PROVIDES})
	if( BIT_LENGTH EQUAL 64 )
		set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" )
	else()
		set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386" )
	endif()
	#string( TOLOWER "${CPACK_PACKAGE_NAME}-${OCE_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}" CPACK_PACKAGE_FILE_NAME )
endif()

###############################################################################
# RPM-specific CPack variables.                                               #
# to enable, run cmake with -DCPACK_GENERATOR="RPM", or uncomment             #
# set(CPACK_GENERATOR "RPM")                                                  #
###############################################################################
if( CPACK_GENERATOR MATCHES ".*RPM.*" )
    # c and c++ runtime should be skipped:  libstdc++6 >= 4.4.0, libc6 >= 2.3, libgcc >= 4.1.1, libgomp >= 4.2.1
    set( CPACK_RPM_PACKAGE_REQUIRES "cpprest-devel")
	set( CPACK_RPM_PACKAGE_PROVIDES ${SAL_PACKAGE_PROVIDES})
	set( CPACK_PACKAGE_RELOCATABLE "FALSE" )
	if( BIT_LENGTH EQUAL 64 )
		set( CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64" )
	else()
		set( CPACK_RPM_PACKAGE_ARCHITECTURE "i586" )
	endif()
	#CPackRPM ignores CPACK_RPM_FILE_NAME. Must set two other vars instead, and they interfere
	#string( TOLOWER "${CPACK_PACKAGE_NAME}-${OCE_VERSION}_${CPACK_RPM_PACKAGE_ARCHITECTURE}.rpm" CPACK_OUTPUT_FILE_NAME )
	if( NOT CPACK_GENERATOR STREQUAL "RPM" )
		message( WARNING "Generating an RPM with other package type(s). Due to a bug in CPackRPM, the package will not get copied to the current directory. It will be found in _CPack_Packages/Linux/RPM, and other packages may fail to build." )
	else()
		#this variable is used by other packagers, don't set it unless RPM is built by itself.
		set( CPACK_PACKAGE_FILE_NAME ${CPACK_OUTPUT_FILE_NAME} )
	endif()
endif()

# finally include CPack
include(CPack)


############################################################
# Create doxygen documentation `make doc`
############################################################
# add a target to generate API documentation with Doxygen
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    # by default it is installed to PREFIX/share/doc,  skip install if online only
    #install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION doc)
endif()