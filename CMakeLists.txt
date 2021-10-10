# JNativeHook: Global keyboard and mouse listeners for Java.
# Copyright (C) 2006-2021 Alexander Barker.  All Rights Reserved.
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

if(NOT CMAKE_SYSTEM_NAME STREQUAL "")
    string(TOLOWER ${CMAKE_SYSTEM_NAME} TARGET_OS)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/src/main/resources/com/github/kwhat/jnativehook/lib/${TARGET_OS}" CACHE PATH "Internal var" FORCE)
else()
    message(FATAL_ERROR "Could not determine target system.  Please set CMAKE_SYSTEM_NAME to the desired resource folder.")
endif()


string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} TARGET_PROCESSOR)
if(TARGET_PROCESSOR STREQUAL x86_64 OR TARGET_PROCESSOR STREQUAL amd64)
    set(CMAKE_INSTALL_LIBDIR "x86_64" CACHE PATH "Internal var" FORCE)
elseif(TARGET_PROCESSOR STREQUAL x86 OR TARGET_PROCESSOR MATCHES "i.86")
    set(CMAKE_INSTALL_LIBDIR "x86" CACHE PATH "Internal var" FORCE)
elseif(TARGET_PROCESSOR MATCHES "^arm" OR TARGET_PROCESSOR MATCHES "^aarch")
    if(TARGET_PROCESSOR MATCHES "64$")
        set(CMAKE_INSTALL_LIBDIR "arm64" CACHE PATH "Internal var" FORCE)
    else()
        set(CMAKE_INSTALL_LIBDIR "arm" CACHE PATH "Internal var" FORCE)
    endif()
elseif(TARGET_PROCESSOR MATCHES "^ppc" OR TARGET_PROCESSOR MATCHES "^power")
    if(TARGET_PROCESSOR MATCHES "64$")
        set(CMAKE_INSTALL_LIBDIR "ppc64" CACHE PATH "Internal var" FORCE)
    else()
        set(CMAKE_INSTALL_LIBDIR "ppc" CACHE PATH "Internal var" FORCE)
    endif()
elseif(NOT CMAKE_SYSTEM_NAME STREQUAL "")
    message(FATAL_ERROR "Unsupported target architecture \"${CMAKE_SYSTEM_PROCESSOR}\".")
else()
    message(FATAL_ERROR "Could not determine target architecture.  Please set CMAKE_SYSTEM_PROCESSOR to the desired resource folder.")
endif()

if(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.5")
endif()


find_package(JNI)
if(NOT JNI_FOUND)
    message(FATAL_ERROR "Could not determine Java include path.  Please set your JAVA_HOME environment variable to the location of the desired JDK.")
endif()

add_library(jnativehook SHARED
    "./src/main/jni/jni_Converter.c"
    "./src/main/jni/jni_Errors.c"
    "./src/main/jni/jni_EventDispatcher.c"
    "./src/main/jni/jni_Globals.c"
    "./src/main/jni/jni_Load.c"
    "./src/main/jni/jni_Logger.c"
    "./src/main/jni/com_github_kwhat_jnativehook_GlobalScreen.c"
)

set_target_properties(jnativehook PROPERTIES
    PROPERTY C_STANDARD 99
    PROPERTY C_STANDARD_REQUIRED ON
    OUTPUT_NAME ${CMAKE_PROJECT_NAME}
)

find_package(uiohook REQUIRED)
target_link_libraries(jnativehook uiohook)

target_include_directories(jnativehook
    PUBLIC
        $<BUILD_INTERFACE:${JNI_INCLUDE_DIRS}>

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jni
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jni/include
)

if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(jnativehook "${CMAKE_THREAD_LIBS_INIT}")
endif()

install(TARGETS jnativehook RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR})
