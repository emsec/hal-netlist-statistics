cmake_minimum_required(VERSION 3.8)

if(POLICY CMP0048)
    # cmake warns if loaded from a min-3.0-required parent dir, so silence the warning:
    cmake_policy(SET CMP0048 NEW)
endif()

message(STATUS "cmake version: ${CMAKE_VERSION}")

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

project(netlist_statistics)

# Check if pybind11 is being used directly or via add_subdirectory
set(HAL_NL_STATISTICS_MASTER_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(HAL_NL_STATISTICS_MASTER_PROJECT ON)
endif()

if(HAL_NL_STATISTICS_MASTER_PROJECT)
    find_package(hal REQUIRED)
    include(hal_cmake_tools)
    option(USE_VERSION_FROM_OPTION "USE_VERSION_FROM_OPTION" FALSE)
    if(NOT ${USE_VERSION_FROM_OPTION})
        hal_plugins_get_version()
    else()
        set(HAL_PLUGINS_VERSION_RETURN ${HAL_PLUGINS_VERSION_MAJOR}.${HAL_PLUGINS_VERSION_MINOR}.${HAL_PLUGINS_VERSION_PATCH})
    endif()
    unset(HAL_PLUGINS_VERSION_MAJOR_SELF CACHE)

    if(UNIX AND NOT APPLE)
        set(LINUX TRUE)
    endif()

    #Install Directive target directiories
    setup_output_directories()

    option(BUILD_ALL_PLUGINS "BUILD_ALL_PLUGINS" OFF)

endif()

# Define the option to enable or disable the build of this plugin by a cmake flag.
option(PL_NETLIST_STATISTICS "PL_NETLIST_STATISTICS" ON)
if(PL_NETLIST_STATISTICS OR BUILD_ALL_PLUGINS)
    # Define the Header and sources files
    set(netlist_statistics_INC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/plugin_netlist_statistics.h
        )

    set(netlist_statistics_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/plugin_netlist_statistics.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/python/python_definitions.cpp
        )

    hal_add_plugin(netlist_statistics
                   SHARED
                   SOURCES ${netlist_statistics_SRC}
                   HEADER ${netlist_statistics_INC})

endif()
