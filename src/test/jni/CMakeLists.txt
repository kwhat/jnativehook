# JNativeHook JNI regression fixtures.
cmake_minimum_required(VERSION 3.10)

project(JNativeHookJniRegressionTests LANGUAGES C)

get_filename_component(REPOSITORY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../.." ABSOLUTE)
set(JAVA_HOME "${JAVA_HOME}" CACHE PATH "JDK used to compile and run JNI fixtures")
set(JAVA_CLASSES_DIR "${REPOSITORY_ROOT}/target/classes" CACHE PATH "Compiled project classes")
set(JNI_GENERATED_INCLUDE "${REPOSITORY_ROOT}/target/include" CACHE PATH "Generated JNI headers")
set(LIBUIOHOOK_INCLUDE "${REPOSITORY_ROOT}/src/external/libuiohook/include" CACHE PATH "libuiohook include directory")

if(NOT JAVA_HOME)
    find_package(Java REQUIRED COMPONENTS Runtime Development)
    set(JAVA_HOME "${Java_JAVA_HOME}")
endif()

set(Java_JAVA_EXECUTABLE "${JAVA_HOME}/bin/java")
set(Java_JAVAC_EXECUTABLE "${JAVA_HOME}/bin/javac")
find_package(JNI REQUIRED)
find_package(Threads REQUIRED)

enable_testing()

set(TEST_CLASSES_DIR "${CMAKE_CURRENT_BINARY_DIR}/classes")
file(GLOB TEST_JAVA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*Test.java")
add_custom_target(jni-regression-test-classes ALL
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${TEST_CLASSES_DIR}"
    COMMAND "${Java_JAVAC_EXECUTABLE}" -source 8 -target 8 -Xlint:-options
        -classpath "${JAVA_CLASSES_DIR}"
        -d "${TEST_CLASSES_DIR}"
        ${TEST_JAVA_SOURCES}
    DEPENDS ${TEST_JAVA_SOURCES}
    VERBATIM
)

set(JNI_TEST_SOURCES
    "${REPOSITORY_ROOT}/src/main/jni/jni_Converter.c"
    "${REPOSITORY_ROOT}/src/main/jni/jni_Errors.c"
    "${REPOSITORY_ROOT}/src/main/jni/jni_EventDispatcher.c"
    "${REPOSITORY_ROOT}/src/main/jni/jni_EventPoster.c"
    "${REPOSITORY_ROOT}/src/main/jni/jni_Globals.c"
    "${REPOSITORY_ROOT}/src/main/jni/jni_Load.c"
    "${REPOSITORY_ROOT}/src/main/jni/jni_Logger.c"
    "${REPOSITORY_ROOT}/src/main/jni/com_github_kwhat_jnativehook_GlobalScreen.c"
)

set(JNI_TEST_INCLUDE_DIRS
    ${JNI_INCLUDE_DIRS}
    "${LIBUIOHOOK_INCLUDE}"
    "${REPOSITORY_ROOT}/src/main/jni"
    "${REPOSITORY_ROOT}/src/main/jni/include"
    "${JNI_GENERATED_INCLUDE}"
)

set(JAVA_TEST_OPTIONS
    -Xcheck:jni
    -Djava.awt.headless=true
)
execute_process(
    COMMAND "${Java_JAVA_EXECUTABLE}" --enable-native-access=ALL-UNNAMED -version
    RESULT_VARIABLE JAVA_NATIVE_ACCESS_RESULT
    OUTPUT_QUIET
    ERROR_QUIET
)
if(JAVA_NATIVE_ACCESS_RESULT EQUAL 0)
    list(APPEND JAVA_TEST_OPTIONS --enable-native-access=ALL-UNNAMED)
endif()

foreach(TEST_JAVA_SOURCE IN LISTS TEST_JAVA_SOURCES)
    get_filename_component(TEST_NAME "${TEST_JAVA_SOURCE}" NAME_WE)
    set(TEST_STUB "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME}_stub.c")

    if(EXISTS "${TEST_STUB}")
        set(TEST_LIBRARY_TARGET "${TEST_NAME}-jni-fixture")
        add_library(${TEST_LIBRARY_TARGET} SHARED ${JNI_TEST_SOURCES} "${TEST_STUB}")
        add_dependencies(${TEST_LIBRARY_TARGET} jni-regression-test-classes)
        set_target_properties(${TEST_LIBRARY_TARGET} PROPERTIES
            C_STANDARD 99
            C_STANDARD_REQUIRED ON
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}"
            OUTPUT_NAME JNativeHook
        )
        target_include_directories(${TEST_LIBRARY_TARGET} PRIVATE ${JNI_TEST_INCLUDE_DIRS})
        target_link_libraries(${TEST_LIBRARY_TARGET} PRIVATE Threads::Threads)
        set_property(TARGET ${TEST_LIBRARY_TARGET} APPEND_STRING PROPERTY LINK_FLAGS " -Wl,-z,defs")

        add_test(NAME ${TEST_NAME}
            COMMAND "${Java_JAVA_EXECUTABLE}" ${JAVA_TEST_OPTIONS}
                "-Djava.library.path=${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}"
                -classpath "${JAVA_CLASSES_DIR}:${TEST_CLASSES_DIR}"
                "${TEST_NAME}"
        )
        set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 30)

        if(TEST_NAME STREQUAL "InitializationTest")
            foreach(ALLOCATION RANGE 1 12)
                add_test(NAME "${TEST_NAME}-allocation-${ALLOCATION}"
                    COMMAND "${Java_JAVA_EXECUTABLE}" ${JAVA_TEST_OPTIONS}
                        "-Djava.library.path=${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}"
                        -classpath "${JAVA_CLASSES_DIR}:${TEST_CLASSES_DIR}"
                        "${TEST_NAME}"
                )
                set_tests_properties("${TEST_NAME}-allocation-${ALLOCATION}" PROPERTIES
                    TIMEOUT 30
                    ENVIRONMENT "JNATIVEHOOK_FAIL_ALLOCATION=${ALLOCATION}"
                )
            endforeach()
            set_property(TARGET ${TEST_LIBRARY_TARGET} APPEND PROPERTY COMPILE_DEFINITIONS calloc=test_calloc)
        endif()
    endif()
endforeach()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/keycode_test.c")
    add_executable(keycode-regression-test
        "${REPOSITORY_ROOT}/src/main/jni/jni_Converter.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/keycode_test.c"
    )
    target_include_directories(keycode-regression-test PRIVATE ${JNI_TEST_INCLUDE_DIRS})
    add_test(NAME keycode-regression-test COMMAND keycode-regression-test)
endif()
