# JNativeHook: Global keyboard and mouse hooking for Java.
# Copyright (C) 2006-2020 Alexander Barker.  All Rights Received.
# https://github.com/kwhat/jnativehook/

# JNativeHook is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# JNativeHook is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.10)

project(JNativeHook VERSION 2.2.0 LANGUAGES C)

if(CMAKE_SYSTEM_NAME STREQUAL "")
    message(FATAL_ERROR "Could not determine target system.  Please set CMAKE_SYSTEM_NAME to the desired resource folder.")
endif()

string(TOLOWER ${CMAKE_SYSTEM_NAME} TARGET_OS)
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/src/main/resources/org/jnativehook/lib/${TARGET_OS}" CACHE PATH "Internal var" FORCE)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL x86_64 OR CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL amd64)
    set(CMAKE_INSTALL_LIBDIR "x86_64" CACHE PATH "Internal var" FORCE)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i.86")
    set(CMAKE_INSTALL_LIBDIR "x86" CACHE PATH "Internal var" FORCE)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^arm" OR CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "64$")
        set(CMAKE_INSTALL_LIBDIR "arm64" CACHE PATH "Internal var" FORCE)
    else()
        set(CMAKE_INSTALL_LIBDIR "arm" CACHE PATH "Internal var" FORCE)
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^ppc" OR CMAKE_SYSTEM_PROCESSOR MATCHES "^(P|p)ower")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "64$")
        set(CMAKE_INSTALL_LIBDIR "ppc64" CACHE PATH "Internal var" FORCE)
    else()
        set(CMAKE_INSTALL_LIBDIR "ppc" CACHE PATH "Internal var" FORCE)
    endif()
else()
    message(FATAL_ERROR "Could not determine target architecture.  Please set CMAKE_SYSTEM_PROCESSOR to the desired resource folder.")
endif()


set(UIOHOOK_ENABLE_DEMO OFF CACHE INTERNAL "Internal var")
set(UIOHOOK_ENABLE_TEST OFF CACHE INTERNAL "Internal var")
set(UIOHOOK_ENABLE_SHARED OFF CACHE INTERNAL "Internal var")
set(UIOHOOK_ENABLE_STATIC OFF CACHE INTERNAL "Internal var")

add_subdirectory(${PROJECT_SOURCE_DIR}/src/external/libuiohook)


find_package(JNI)
if (NOT JNI_FOUND)
    message(FATAL_ERROR "Could not determine Java include path.  Please set your JAVA_HOME environment variable to the location of the desired JDK.")
endif()

add_library(jnativehook OBJECT
    "./src/main/jni/jni_Converter.c"
    "./src/main/jni/jni_Errors.c"
    "./src/main/jni/jni_EventDispatcher.c"
    "./src/main/jni/jni_Globals.c"
    "./src/main/jni/jni_Load.c"
    "./src/main/jni/jni_Logger.c"
    "./src/main/jni/org_jnativehook_GlobalScreen.c"
)

set_property(TARGET jnativehook PROPERTY POSITION_INDEPENDENT_CODE 1)
set_property(TARGET jnativehook PROPERTY C_STANDARD 99)
set_property(TARGET jnativehook PROPERTY C_STANDARD_REQUIRED ON)

target_include_directories(jnativehook
    PUBLIC
        $<BUILD_INTERFACE:${JNI_INCLUDE_DIRS}>

    PRIVATE
        $<TARGET_PROPERTY:uiohook,INTERFACE_INCLUDE_DIRECTORIES>
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jni
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jni/include
        ${CMAKE_CURRENT_SOURCE_DIR}/target/generated-sources/include
)

add_library(jnativehook_shared SHARED $<TARGET_OBJECTS:jnativehook>)
set_target_properties(jnativehook_shared PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})
install(TARGETS jnativehook_shared DESTINATION ${CMAKE_INSTALL_LIBDIR})
target_link_libraries(jnativehook_shared uiohook)

if (UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(jnativehook "${CMAKE_THREAD_LIBS_INIT}")
endif()

set_target_properties(jnativehook PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
