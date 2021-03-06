cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
set(IRODS_VERSION "4.1.x")
message(STATUS "Building for irods-server version ${IRODS_VERSION}")

# Note: Microservices for iRODS >=4.2 need to be built with the compiler and
# standard libarary provided by iRODS-externals packages.
# The following packages need to be installed to build the uu microservices:
#
# irods-externals-clang-runtime3.8-0
# irods-externals-clang3.8-0

project(uu-msis CXX)

add_compile_options(-std=c++11
                    -Os
                    -fPIC
                    -DRODS_SERVER
                    -Wall
                    -Wextra
                    -Wcast-align
                    -Wredundant-decls
                    -Wuninitialized
		    -Wpedantic
                    -Wconversion
                    -Wno-missing-field-initializers
                    -Wno-unused-parameter)

find_package(Boost 1.51 COMPONENTS locale REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

find_package(CURL REQUIRED)
include_directories(SYSTEM ${CURL_INCLUDE_DIRS})

find_package(LibXml2 REQUIRED)
include_directories(SYSTEM ${LIBXML2_INCLUDE_DIR})

find_package(LibXslt REQUIRED)
include_directories(SYSTEM ${LIBXSLT_INCLUDE_DIR})

find_package(Jansson REQUIRED)
include_directories(SYSTEM ${JANSSON_INCLUDE_DIRS})

include_directories(SYSTEM "/usr/include/irods")

add_library(msiSetUpperCaseWhereQuery SHARED src/msiSetUpperCaseWhereQuery.cc)
add_library(msiStrToUpper             SHARED src/msiStrToUpper.cc)
add_library(msiRegisterDataCiteDOI    SHARED src/msiRegisterDataCiteDOI.cc)
add_library(msiGenerateRandomID       SHARED src/msiGenerateRandomID.cc)
add_library(msiGetDataCiteDOI         SHARED src/msiGetDataCiteDOI.cc)
add_library(msiLoadMetadataFromXml    SHARED src/msiLoadMetadataFromXml.cc)
add_library(msiXmlDocSchemaValidate   SHARED src/msiXmlDocSchemaValidate.cc)
add_library(msiXsltApply              SHARED src/msiXsltApply.cc)
add_library(msi_json_arrayops         SHARED src/msi_json_arrayops.cc)
add_library(msi_json_objops           SHARED src/msi_json_objops.cc)

target_link_libraries(msiStrToUpper           LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(msiRegisterDataCiteDOI  LINK_PUBLIC ${CURL_LIBRARIES})
target_link_libraries(msiGetDataCiteDOI       LINK_PUBLIC ${CURL_LIBRARIES})
target_link_libraries(msiLoadMetadataFromXml  LINK_PUBLIC ${LIBXML2_LIBRARIES})
target_link_libraries(msiXmlDocSchemaValidate LINK_PUBLIC ${LIBXML2_LIBRARIES})
target_link_libraries(msiXsltApply            LINK_PUBLIC ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARIES})
target_link_libraries(msi_json_arrayops      LINK_PUBLIC ${JANSSON_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(msi_json_objops      LINK_PUBLIC ${JANSSON_LIBRARIES} ${Boost_LIBRARIES})

install(TARGETS
        msiSetUpperCaseWhereQuery
        msiStrToUpper
	msiRegisterDataCiteDOI
        msiGenerateRandomID
	msiGetDataCiteDOI
	msiLoadMetadataFromXml
	msiXmlDocSchemaValidate
	msiXsltApply
	msi_json_arrayops
	msi_json_objops
        DESTINATION
        /var/lib/irods/plugins/microservices)

# Packaging {{{

# Make sure your umask is set correctly when packaging:
# The default umask on CentOS 7 is 0002, which results in e.g. rwxrwxr-x
# directories within generated RPMs, which will conflict with existing
# directories from irods packages, which will be rwxr-xr-x.
# To create packages on CentOS 7, set your umask to 0022 first (`umask 0022`).

set(CPACK_MONOLITHIC_INSTALL 1)
set(CPACK_CMAKE_GENERATOR "Unix Makefiles")
#set(CPACK_GENERATOR "DEB")
set(CPACK_GENERATOR "RPM")
set(CPACK_PACKAGE_NAME "irods-uu-microservices")
set(CPACK_PACKAGE_VENDOR "Utrecht University <fbyoda@uu.nl>")
set(CPACK_PACKAGE_CONTACT "Utrecht University <fbyoda@uu.nl>")
set(CPACK_PACKAGE_VERSION "4.1.11_0.5.2")

set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/package/description.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Miscellaneous microservices developed by Utrecht university.")

set(CPACK_RESOURCE_FILE_README  "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_RPM_PACKAGE_RELEASE "4")
set(CPACK_RPM_PACKAGE_LICENSE "LGPLv3")
set(CPACK_RPM_PACKAGE_REQUIRES "boost-locale >= 1.51, libcurl >= 7.29.0, libxml2 >= 2.9.1, libxslt >= 1.1.28, jansson >= 2.10")
set(CPACK_RPM_PACKAGE_CONFLICTS "rit-irods-microservices, irods-microservice-plugins-loadmeta")
set(CPACK_RPM_PACKAGE_URL "https://github.com/UtrechtUniversity/irods-uu-microservices")
set(CPACK_RPM_PACKAGE_AUTOREQ 0)
set(CPACK_RPM_PACKAGE_AUTOPROV 0)

set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/UtrechtUniversity/irods-uu-microservices")
set(CPACK_DEBIAN_PACKAGE_SECTION "contrib/science")

set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}")

include(CPack)

# }}}
