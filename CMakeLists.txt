########################################################################
IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    MESSAGE(FATAL_ERROR "Prevented in-tree built. This is bad practice.")
ENDIF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})


########################################################################
# Project setup
########################################################################
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)
PROJECT(mem-utils CXX)

 
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/tools)

MESSAGE(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
