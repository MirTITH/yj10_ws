# Automatically generated by scripts/boost/generate-ports.ps1

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/algorithm
    REF boost-1.83.0
    SHA512 88321824ce0ec8c804c6a8451d16fb45b611107aaf0fcc0ee9ca4efd6fb17e60638b55867c811b8c3e57a270c21573ccba4d7e49865ee7a8df2e87d7759f46e4
    HEAD_REF master
)

include(${CURRENT_INSTALLED_DIR}/share/boost-vcpkg-helpers/boost-modular-headers.cmake)
boost_modular_headers(SOURCE_PATH ${SOURCE_PATH})