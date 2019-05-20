cmake_minimum_required(VERSION 3.8)

if(POLICY CMP0048)
    # cmake warns if loaded from a min-3.0-required parent dir, so silence the warning:
    cmake_policy(SET CMP0048 NEW)
endif()

message(STATUS "cmake version: ${CMAKE_VERSION}")

include(CMakeDependentOption)

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
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
    include(Subdirs)
    option(USE_VERSION_FROM_OPTION "USE_VERSION_FROM_OPTION" FALSE)
    include(HalPluginsUtils)
    include(DetectDistro)
    if(NOT ${USE_VERSION_FROM_OPTION})
        hal_plugins_get_version()
    else()
        set(HAL_PLUGINS_VERSION_RETURN ${HAL_PLUGINS_VERSION_MAJOR}.${HAL_PLUGINS_VERSION_MINOR}.${HAL_PLUGINS_VERSION_PATCH})
    endif()
    unset(HAL_PLUGINS_VERSION_MAJOR_SELF CACHE)

    if(UNIX AND NOT APPLE)
        set(LINUX TRUE)
    endif()

    #Use C11 and C++17 as minimum standard
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED on)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED on)

    # declare project output binary path
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
    foreach(i IN ITEMS "RUNTIME" "ARCHIVE" "LIBRARY")
        #    if(LINUX)
        set(appendix "/lib")
        if(i STREQUAL "RUNTIME")
            set(appendix "/bin")
        endif(i STREQUAL "RUNTIME")
        #    elseif(APPLE)
        #        set(appendix "/bin")
        #    endif(LINUX)
        if(NOT CMAKE_${i}_OUTPUT_DIRECTORY)
            set(CMAKE_${i}_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}${appendix}")
        elseif()
            set(CMAKE_${i}_OUTPUT_DIRECTORY "${CMAKE_${i}_OUTPUT_DIRECTORY}${appendix}")
        endif()
    endforeach()

    foreach(i IN ITEMS "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO")
        foreach(j IN ITEMS "RUNTIME" "ARCHIVE" "LIBRARY")
            if(NOT CMAKE_${j}_OUTPUT_DIRECTORY${i})
                set(CMAKE_${j}_OUTPUT_DIRECTORY${i} ${CMAKE_${j}_OUTPUT_DIRECTORY})
            endif()
        endforeach()
    endforeach()

    #Install Directive target directiories
    message(STATUS "")
    include(GNUInstallDirs)
    set(INCLUDE_INSTALL_DIRECTORY ${CMAKE_INSTALL_INCLUDEDIR}/hal)
    message(STATUS "INCLUDE_INSTALL_DIRECTORY: ${INCLUDE_INSTALL_DIRECTORY}")

    set(INCLUDE_INSTALL_DIRECTORY_FULL ${CMAKE_INSTALL_FULL_INCLUDEDIR}/hal)
    message(STATUS "INCLUDE_INSTALL_DIRECTORY_FULL: ${INCLUDE_INSTALL_DIRECTORY_FULL}")

    set(SHARE_INSTALL_DIRECTORY ${CMAKE_INSTALL_DATADIR}/hal)
    message(STATUS "SHARE_INSTALL_DIRECTORY: ${SHARE_INSTALL_DIRECTORY}")

    set(SHARE_INSTALL_DIRECTORY_FULL ${CMAKE_INSTALL_FULL_DATADIR}/hal)
    message(STATUS "SHARE_INSTALL_DIRECTORY_FULL: ${SHARE_INSTALL_DIRECTORY_FULL}")

    set(PKGCONFIG_INSTALL_DIRECTORY ${CMAKE_INSTALL_DATADIR}/pkgconfig)
    message(STATUS "PKGCONFIG_INSTALL_DIRECTORY: ${PKGCONFIG_INSTALL_DIRECTORY}")

    set(BINARY_INSTALL_DIRECTORY ${CMAKE_INSTALL_BINDIR})
    message(STATUS "BINARY_INSTALL_DIRECTORY: ${BINARY_INSTALL_DIRECTORY}")

    set(BINARY_INSTALL_DIRECTORY_FULL ${CMAKE_INSTALL_FULL_BINDIR})
    message(STATUS "BINARY_INSTALL_DIRECTORY_FULL: ${BINARY_INSTALL_DIRECTORY_FULL}")

    set(LIBRARY_INSTALL_DIRECTORY ${CMAKE_INSTALL_LIBDIR})
    message(STATUS "LIBRARY_INSTALL_DIRECTORY: ${LIBRARY_INSTALL_DIRECTORY}")

    set(LIBRARY_INSTALL_DIRECTORY_FULL ${CMAKE_INSTALL_FULL_LIBDIR})
    message(STATUS "LIBRARY_INSTALL_DIRECTORY_FULL: ${LIBRARY_INSTALL_DIRECTORY_FULL}")

    set(PLUGIN_INCLUDE_INSTALL_DIRECTORY ${INCLUDE_INSTALL_DIRECTORY}/plugins)
    message(STATUS "PLUGIN_INCLUDE_INSTALL_DIRECTORY: ${PLUGIN_INCLUDE_INSTALL_DIRECTORY}")
    set(PLUGIN_BINARY_INSTALL_DIRECTORY ${BINARY_INSTALL_DIRECTORY})
    message(STATUS "PLUGIN_BINARY_INSTALL_DIRECTORY: ${PLUGIN_BINARY_INSTALL_DIRECTORY}")
    set(PLUGIN_LIBRARY_INSTALL_DIRECTORY ${LIBRARY_INSTALL_DIRECTORY}/hal_plugins)
    message(STATUS "PLUGIN_LIBRARY_INSTALL_DIRECTORY: ${PLUGIN_LIBRARY_INSTALL_DIRECTORY}")

    option(BUILD_ALL_PLUGINS "BUILD_ALL_PLUGINS" OFF)

    find_package(Hal REQUIRED)
    set(LINK_LIBS ${HAL_LIBRARIES})
    include_directories(${Hal_INCLUDE_DIR}/hal SYSTEM ${PYBIND11_INCLUDE_DIR})
    message(STATUS "Hal_INCLUDE_DIR: ${Hal_INCLUDE_DIR}")

    # Need Version 2.2.4 Not available in ubuntu bionic
    find_package(pybind11 CONFIG)
    message(STATUS "Found pybind11 v${pybind11_VERSION}: ${pybind11_INCLUDE_DIRS}")
    if(${pybind11_VERSION} VERSION_GREATER_EQUAL 2.2.4)
        message(STATUS "Found pybind11 >= 2.2.4")
        include_directories(pybind11_INCLIDE_DIRS)
    else()
        message(STATUS "Found pybind11 < 2.2.4")
        add_subdirectory(deps/pybind11)
    endif()

    find_package(spdlog CONFIG)
    message(STATUS "Found spdlog v${spdlog_VERSION}")
    if(${spdlog_VERSION} VERSION_LESS 1.3.1)
        message(STATUS "Found spdlog < 1.3.1")
        include_directories(${include_directories} "${CMAKE_CURRENT_SOURCE_DIR}/deps/spdlog-1.3.1/include")
        set(spdlog_VERSION 1.3.1)
    else()
        message(STATUS "Found spdlog >= 1.3.1")
    endif()
endif()

# Define the option to enable or disable the build of this plugin by a cmake flag.
option(PL_NETLIST_STATISTICS "PL_NETLIST_STATISTICS" ON)
if(PL_NETLIST_STATISTICS OR BUILD_ALL_PLUGINS)
    # Add the include directory to the include search path
    include_directories(AFTER "${CMAKE_CURRENT_SOURCE_DIR}/include")

    # Define the Header and sources files
    set(netlist_statistics_INC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/factory_netlist_statistics.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/plugin_netlist_statistics.h
        )

    set(netlist_statistics_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/factory_netlist_statistics.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/plugin_netlist_statistics.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/python/python_definitions.cpp
        )

    # Create the '__init__.py' file in the hal_plugins directory as there might not be a any plugins available yet.
    file(WRITE ${CMAKE_BINARY_DIR}/lib/hal_plugins/__init__.py "")

    # Create the plugin
    add_library(netlist_statistics SHARED ${netlist_statistics_SRC} ${netlist_statistics_INC})

    # To allow shared libraries to be loaded by python on macOS you need to path the suffix of the lib file.
    if(APPLE AND CMAKE_HOST_APPLE)
        set_target_properties(netlist_statistics PROPERTIES SUFFIX ".so")
        set_target_properties(graph_algorithm PROPERTIES
                              INSTALL_NAME_DIR ${PLUGIN_LIBRARY_INSTALL_DIRECTORY}
                              )
    endif(APPLE AND CMAKE_HOST_APPLE)

    # Link the plugin against the pybind11 and python libraries as well as the HAL libraries.
    target_link_libraries(netlist_statistics ${PYTHON_LIBRARIES} pybind11::pybind11 ${LINK_LIBS})

    # Install the library to the appropriate directories
    install(TARGETS netlist_statistics LIBRARY DESTINATION ${PLUGIN_LIBRARY_INSTALL_DIRECTORY} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE INCLUDES DESTINATION ${PLUGIN_INCLUDE_INSTALL_DIRECTORY})

    # Install the library header to the appropriate directories
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${PLUGIN_INCLUDE_INSTALL_DIRECTORY}/netlist_statistics/include/)

    # Add sanitizers if you build in Debug mode
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        add_sanitizers(netlist_statistics)
    endif()

    # Make sure the plugin is build while running e.g. 'make all'.
    add_dependencies(netlist_statistics ${CMAKE_PROJECT_NAME} ${PYTHON_BINDING_LIB})
endif()
