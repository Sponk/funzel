cmake_minimum_required(VERSION 3.22)

if(NOT FUNZEL_NO_VCPKG AND EXISTS ${CMAKE_SOURCE_DIR}/vcpkg AND "${VCPKG_ROOT}" STREQUAL "")
	set(VCPKG_ROOT ${CMAKE_SOURCE_DIR}/vcpkg)
endif()

if(NOT "${VCPKG_ROOT}" STREQUAL "")
	if(WIN32)
		set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "The VCPKG target triplet to use.")
	else()
		set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "The VCPKG target triplet to use.")
	endif()

	set(CMAKE_TOOLCHAIN_FILE
			"${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
			CACHE PATH "The VCPKG installation to use.")

	set(USE_VCPKG TRUE)

	message("-- Using VCPKG at: ${VCPKG_ROOT}")
	message("-- Using target triplet: ${VCPKG_TARGET_TRIPLET}")
endif()

project(
	Funzel
	VERSION 0.1
	DESCRIPTION "A small, GPU enabled tensor library for C++."
)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

## Set output directories so all artifacts are easily found
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(FUNZEL_ENABLE_LTO "Use LTO if available." ON)

option(USE_RELATIVE_RPATH "Use relative RPATH on *NIX systems." ON)
if(USE_RELATIVE_RPATH AND NOT APPLE)
	set(CMAKE_SKIP_BUILD_RPATH  FALSE)
	set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
	set(CMAKE_INSTALL_RPATH "$ORIGIN" "$ORIGIN/../lib")
endif()

option(FUNZEL_USE_LIBCPP "Use LLVM libc++ instead of libstdc++." OFF)
if (FUNZEL_USE_LIBCPP AND CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND NOT ANDROID)
	add_compile_options(-stdlib=libc++)
	add_link_options(-stdlib=libc++)
endif()

option(FUNZEL_ENABLE_AVX2 "Use AVX2 instruction set." ON)
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(FUNZEL_ARCH "x86-64-v3" CACHE STRING "The CPU architecture to generate code for. Equivalent to '-march=<arch>'")
	if(FUNZEL_ENABLE_AVX2)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mavx2")
	endif()

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O3 -funroll-loops -march=${FUNZEL_ARCH}")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g -O3 -funroll-loops -march=${FUNZEL_ARCH}")
elseif(MSVC)
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Ox /Ob2 /Oi /Ot")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Ob2 /Oi /Ot")

	if(FUNZEL_ENABLE_AVX2)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:AVX2")
	endif()
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(FUNZEL_ENABLE_SANITIZERS "Enable Clang sanitizers." OFF)

if(WIN32)
	include(DownloadSwig)
endif()

# TODO: MinGW support?
if(NOT WIN32)
	option(ENABLE_GCOV "Enable GCOV coverage." OFF)
	if(ENABLE_GCOV)
		message("-- Enabled GCOV.")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
	endif()

	option(ENABLE_PROFILING "Enable profiling." OFF)
	if(ENABLE_PROFILING)
		message("-- Enabled profiling.")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
	endif()
endif()

add_subdirectory(libfunzel)
add_subdirectory (docs)

option(FUNZEL_NO_EXAMPLES "Disables building examples." OFF)
if(NOT FUNZEL_NO_EXAMPLES)
	add_subdirectory(examples)
endif()
