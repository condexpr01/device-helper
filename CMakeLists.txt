# debug
if(OFF)
	set(CMAKE_CXX_COMPILER clang++)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
	set(CMAKE_BUILD_TYPE Debug)
else()
	set(CMAKE_BUILD_TYPE Release)
endif()

#set(SDL3_DIR SDL3/build)

#cross compile
if(CROSS_MINGW OR CMAKE_HOST_SYSTEM_NAME MATCHES "MSYS|CYGWIN")
	set(CROSS_MINGW ON)
	message(STATUS "cross mingw")

	#libromfs flags
	set(NATIVE_CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
	set(NATIVE_CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
	set(NATIVE_CMAKE_C_FLAGS "-static")
	set(NATIVE_CMAKE_CXX_FLAGS "-static")

	set(CMAKE_SYSTEM_NAME Windows)
	set(CMAKE_SYSTEM_PROCESSOR x86_64)
	set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
	set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

	#cross compile on linux
	if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
		message(STATUS "linux x86_64-w64-mingw32")
		set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
		set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
		set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
		set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
		set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
	endif()

endif()

#proj, ver
set(PROJECT_NAME "prog")
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 26)
cmake_minimum_required(VERSION 4.0)
project(${PROJECT_NAME} CXX)

#msvc utf8
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

#executable target
add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PRIVATE include
	PRIVATE include/imgui
	PRIVATE include/core
	PRIVATE include/core/gl
	PRIVATE include/core/sdl
	PRIVATE include/core/std
	PRIVATE include/glad
	PRIVATE include/stb
	PRIVATE include/exprtk
	#PRIVATE glm
)

target_sources(${PROJECT_NAME}
	PRIVATE src/core/main.cpp
	PRIVATE src/core/loop.cpp

	PRIVATE src/imgui/imgui.cpp
	PRIVATE src/imgui/imgui_demo.cpp
	PRIVATE src/imgui/imgui_draw.cpp
	PRIVATE src/imgui/imgui_tables.cpp
	PRIVATE src/imgui/imgui_widgets.cpp
	PRIVATE src/imgui/imgui_impl_opengl3.cpp
	PRIVATE src/imgui/imgui_impl_sdl3.cpp

	PRIVATE src/imgui/imgui_stdlib.cpp

	#PRIVATE src/imgui/implot.cpp
	#PRIVATE src/imgui/implot_demo.cpp
	#PRIVATE src/imgui/implot_items.cpp

	#PRIVATE src/imgui/imgui_md.cpp

	#PRIVATE src/imgui/implot3d.cpp
	#PRIVATE src/imgui/implot3d_demo.cpp
	#PRIVATE src/imgui/implot3d_items.cpp
	#PRIVATE src/imgui/implot3d_meshes.cpp

	PRIVATE src/imgui/imgui_toggle.cpp
	PRIVATE src/imgui/imgui_toggle_palette.cpp
	PRIVATE src/imgui/imgui_toggle_presets.cpp
	PRIVATE src/imgui/imgui_toggle_renderer.cpp

	#PRIVATE src/imgui/imnodes.cpp

	PRIVATE src/glad/gl.cpp

	#PRIVATE src/stb/stb_vorbis.c
	PRIVATE src/stb/impl/stb_image_write.cpp
)


#libromfs
set(LIBROMFS_PROJECT_NAME ${PROJECT_NAME})
set(LIBROMFS_RESOURCE_LOCATION "${CMAKE_SOURCE_DIR}/romfs")
add_subdirectory(libromfs)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBROMFS_LIBRARY})

#gl
find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
	target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
endif()

########include fetch content#######
#set(GIT_EXECUTABLE C:\\msys64\\usr\\bin\\git.exe)
include(FetchContent)

#sdl3
if(WIN32)
	set(SDL_STATIC ON)
endif()

find_package(SDL3)
if(NOT SDL3_FOUND)
	FetchContent_Declare(SDL3
		GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
		#GIT_REPOSITORY git@github.com:libsdl-org/SDL.git
		#GIT_REPOSITORY file:///home/condexpr01/Desktop/fakerepos/clone/SDL/.git
		GIT_TAG release-3.4.8
		)

	FetchContent_MakeAvailable(SDL3)
endif()

# sdl related
if(WIN32)
	if(MSVC)
		target_link_options(
			${PROJECT_NAME} 
			PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup
		)
	else()
		target_link_options(${PROJECT_NAME} PRIVATE -static -mwindows)
	endif()

	target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-static)

else()
	target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
endif()



#glm
find_package(glm)
if(NOT glm_FOUND)
	FetchContent_Declare(glm
		GIT_REPOSITORY https://github.com/g-truc/glm.git
		#GIT_REPOSITORY git@github.com:g-truc/glm.git
		#GIT_REPOSITORY file:///home/condexpr01/Desktop/fakerepos/clone/glm/.git
		GIT_TAG 1.0.3
		)
	FetchContent_MakeAvailable(glm)
	target_include_directories(${PROJECT_NAME} PRIVATE ${glm_SOURCE_DIR})
endif()


#pcap
if(WIN32)
	set(PCAP_FOUND ON) #prevent fetch

	if(PCAP_ROOT)
		target_include_directories(${PROJECT_NAME}
			PRIVATE ${PCAP_ROOT}/Include
		)

		target_link_directories(${PROJECT_NAME}
			PRIVATE ${PCAP_ROOT}/Lib/x64
		)

		target_link_libraries(${PROJECT_NAME} PRIVATE wpcap.lib Packet.lib ws2_32)
	else()

		message(FATAL_ERROR "(Windows)compile needs -DPCAP_ROOT=<NPcap-SDK-dir>")
		RETURN()

	endif()

else()

	find_package(PkgConfig)
	if(PkgConfig_FOUND)
		pkg_check_modules(PCAP IMPORTED_TARGET libpcap)
	endif()

	if(PCAP_FOUND)
		target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::PCAP)
	endif()

endif()

if(NOT PCAP_FOUND)
	FetchContent_Declare(pcap
		GIT_REPOSITORY https://github.com/the-tcpdump-group/libpcap
		#GIT_REPOSITORY git@github.com:the-tcpdump-group/libpcap
		#GIT_REPOSITORY file:///home/condexpr01/Desktop/fakerepos/clone/libpcap/.git
		GIT_TAG master
		)

	FetchContent_MakeAvailable(pcap)

	#target_link_libraries(${PROJECT_NAME} PRIVATE pcap)
	target_link_libraries(${PROJECT_NAME} PRIVATE pcap_static)
endif()

