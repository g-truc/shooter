cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_policy(VERSION 2.6)

project("shoot(r)")

######################
# Add build directory

execute_process(COMMAND cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Release)
execute_process(COMMAND cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Debug)

###########
# Add SDL 

if(UNIX)
	if(DEFINED CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
	else()
		set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
	endif()

    execute_process(
		COMMAND ./autogen.sh
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl-1.3.0-5206)
    execute_process(
		COMMAND ./configure --prefix=${CMAKE_CURRENT_SOURCE_DIR}/external/sdl-1.3.0-5206
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl-1.3.0-5206)
    execute_process(
		COMMAND make
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl-1.3.0-5206)
    execute_process(
		COMMAND make install
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl-1.3.0-5206)
    link_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/sdl-1.3.0-5206/lib")
else(UNIX)
	if(MSVC_VERSION EQUAL 1400)
		link_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/sdl-1.3.0-5206/binary/win32-vc2005")
		execute_process(COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl-1.3.0-5206/binary/win32-vc2005/SDL.dll ${CMAKE_CURRENT_BINARY_DIR}/Release/SDL.dll)
		execute_process(COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl-1.3.0-5206/binary/win32-vc2005/SDL.dll ${CMAKE_CURRENT_BINARY_DIR}/Debug/SDL.dll)
	endif(MSVC_VERSION EQUAL 1400)
	if(MSVC_VERSION EQUAL 1500)
		link_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/sdl-1.3.0-5206/binary/win32-vc2008")
		execute_process(COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl-1.3.0-5206/binary/win32-vc2008/SDL.dll ${CMAKE_CURRENT_BINARY_DIR}/Release/SDL.dll)
		execute_process(COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl-1.3.0-5206/binary/win32-vc2008/SDL.dll ${CMAKE_CURRENT_BINARY_DIR}/Debug/SDL.dll)
	endif(MSVC_VERSION EQUAL 1500)	
	if(MSVC_VERSION EQUAL 1600)
		link_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/sdl-1.3.0-5206/binary/win32-vc2010")
		execute_process(COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl-1.3.0-5206/binary/win32-vc2010/SDL.dll ${CMAKE_CURRENT_BINARY_DIR}/Release/SDL.dll)
		execute_process(COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl-1.3.0-5206/binary/win32-vc2010/SDL.dll ${CMAKE_CURRENT_BINARY_DIR}/Debug/SDL.dll)
	endif(MSVC_VERSION EQUAL 1600)
endif(UNIX)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/sdl-1.3.0-5206/include")

###########ð#
# Add GLEW 

if(UNIX)

else(UNIX)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/glew-1.5.1/include")
if(MSVC_VERSION EQUAL 1400)
	link_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/glew-1.5.1/lib/win32-vc2005")
endif(MSVC_VERSION EQUAL 1400)
if(MSVC_VERSION EQUAL 1500)
	link_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/glew-1.5.1/lib/win32-vc2008")
endif(MSVC_VERSION EQUAL 1500)	
if(MSVC_VERSION EQUAL 1600)
	link_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/glew-1.5.1/lib/win32-vc2010")
endif(MSVC_VERSION EQUAL 1600)

endif(UNIX)

###########ð#
# Add FMOD3

if(UNIX)

else(UNIX)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/fmod-3.75/api/inc")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/fmod-3.75/api/lib")

execute_process(COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/fmod-3.75/api/fmod.dll ${CMAKE_CURRENT_BINARY_DIR}/Release/fmod.dll)
execute_process(COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/fmod-3.75/api/fmod.dll ${CMAKE_CURRENT_BINARY_DIR}/Debug/fmod.dll)

endif(UNIX)

###########ð#
# Add libxml2-2.7.6

if(UNIX)

else(UNIX)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/libxml2-2.7.6/include")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/libxml2-2.7.6/lib")

execute_process(COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/libxml2-2.7.6/bin/libxml2.dll ${CMAKE_CURRENT_BINARY_DIR}/Release/libxml2.dll)
execute_process(COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/libxml2-2.7.6/bin/libxml2.dll ${CMAKE_CURRENT_BINARY_DIR}/Debug/libxml2.dll)
execute_process(COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/libxml2-2.7.6/bin/iconv.dll ${CMAKE_CURRENT_BINARY_DIR}/Release/iconv.dll)
execute_process(COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/libxml2-2.7.6/bin/iconv.dll ${CMAKE_CURRENT_BINARY_DIR}/Debug/iconv.dll)
execute_process(COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/libxml2-2.7.6/bin/zlib1.dll ${CMAKE_CURRENT_BINARY_DIR}/Release/zlib1.dll)
execute_process(COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/libxml2-2.7.6/bin/zlib1.dll ${CMAKE_CURRENT_BINARY_DIR}/Debug/zlib1.dll)

endif(UNIX)

###########
# Add GLM 
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/glm-0.9.dev")

###########
# Add GLI
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/gli-0.1.1.0")

####################
# Add source group

source_group("Shader Files" FILES "*.vert" "*.geom" "*.frag")

################################
# Add libraries to executables

if(MSVC)
	set(BINARY_FILES "SDL" "SDLmain" "glew32s" "OpenGL32" "fmodvc" "fmodvc" "libxml2")
else(MSVC)
	set(BINARY_FILES "SDL" "SDLmain" "GL")
endif(MSVC)

################################
# Add output directory

if(MSVC)
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
else(MSVC)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/Debug )
	else(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/Release )
	endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
endif(MSVC)

################################
# Add custom target to copy all data

set(TARGET_DATA_COPY DATA_COPY)
add_custom_target(
	${TARGET_DATA_COPY}
	COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data)

################################
# Add custom target to copy all installer files

set(TARGET_INSTALLER_COPY INSTALLER_COPY)
add_custom_target(
	${TARGET_INSTALLER_COPY}
	COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/installer ${CMAKE_CURRENT_BINARY_DIR}/installer)

################################
# Add definitions

add_definitions(
	-D_CRT_SECURE_NO_WARNINGS)

################################
# CPack

include(CPack)

################################
# Add subdirectory

add_subdirectory(source)

