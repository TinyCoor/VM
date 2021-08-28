cmake_minimum_required(VERSION 3.17)
project(VM VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD  11)

#include(InstallRequiredSystemLibraries)
#set()
#协程
# g++ -fcoroutines -std=c++20
#clang++ -std=c++20 -stdlib=libc++ -fcoroutines-ts
if (WIN32)
	message(STATUS WIN32 Platform)
else()
	message(STATUS Not Support Platform)
endif ()

if (MSVC)
	message(STATUS This is Msvc compiler)
elseif(GNU)
	add_compile_definitions(-Wall -Werror -g -Wswitch-enum -std=c11 -O3 -pedantic)
	message(STATUS This is Not Msvc compiler)
else()
	add_compile_options(-Wall -Werror -g -Wswitch-enum -std=c11 -O3 -pedantic)
	message(STATUS Clang Compiler)
endif ()

set(HEAD
        src/string_view.h
        src/vm.h
		src/assmbler.h
		src/label.h
		src/inst_t.h
		src/error_t.h
		src/native_call.h
		src/utils.h)


set(SRC
        src/string_view.c
        src/vm.c
		src/assmbler.c
        src/label.c
		src/inst_t.c
		src/native_call.c
		src/file.h
		src/file.c
		src/utils.c
		src/error_t.c
		)

add_executable(bmi  src/bme.c ${SRC})

add_executable(nan ./src/nan.c)

add_executable(basm src/main.c ${SRC} )

add_executable(debasm src/disassmebler.c ${SRC})

install(FILES ${HEAD} DESTINATION ${CMAKE_BINARY_DIR}/include)

install(TARGETS bmi
		CONFIGURATIONS Release
		RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/Release/bin)
install(TARGETS basm
		CONFIGURATIONS Release
		RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/Release/bin)
install(TARGETS debasm
		CONFIGURATIONS Release
		RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/Release/bin)

#打包
include(CPack)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDER "XIAOMI")
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR})
set(CPACK_PACKAGE_VERSION_MAJOR 1)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(CPACK_PACKAGE_DESCRIPTION "This is a test")
