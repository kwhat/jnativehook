message(${CMAKE_SOURCE_DIR})

include(FindJava)
include(FindJNI)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/native/common
	${JNI_INCLUDE_DIRS}
)

#configure_file (
#	"${PROJECT_SOURCE_DIR}/NativeConfig.h.in"
#	"${PROJECT_BINARY_DIR}/NativeConfig.h"
#)

#include_directories
set(${PROJECT_NAME}_common_src
	common/NativeErrors.c
	common/org_jnativehook_GlobalScreen.c
)

if (APPLE)
	add_subdirectory(osx)
elseif (UNIX)
	set(${PROJECT_NAME}_platform_src
		unix/JConvertFromNative.c
		unix/NativeHelpers.c
		unix/NativeThread.c
	)
elseif (WIN32)
	add_subdirectory(windows)
else ()
	message("Invalid or unsupported platform.")
endif ()

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_common_src} ${${PROJECT_NAME}_platform_src})
