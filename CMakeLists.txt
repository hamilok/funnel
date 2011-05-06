CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (funnel CXX)

SET (FUNNEL_VERSION_MAJOR 1)
SET (FUNNEL_VERSION_MINOR 0)

CONFIGURE_FILE (
    "${CMAKE_SOURCE_DIR}/include/config.hpp.inc"
    "${CMAKE_SOURCE_DIR}/include/config.hpp"
)

FIND_PACKAGE (Boost 1.42.0 REQUIRED COMPONENTS program_options regex system filesystem thread)
FIND_PACKAGE (OpenSSL REQUIRED)

INCLUDE_DIRECTORIES ("${CMAKE_SOURCE_DIR}/include")

ADD_EXECUTABLE (funnel src/main.cpp src/server.cpp src/zone_manager.cpp src/checksum.cpp src/network_v4.cpp)

TARGET_LINK_LIBRARIES (funnel ${Boost_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARIES})
