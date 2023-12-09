
cmake_minimum_required( VERSION 3.16 )

project( NomCool VERSION 1.0.0 )

message( STATUS "**** Configuring ${PROJECT_NAME} version ${PROJECT_VERSION} ****")

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_compile_options( -Wall -Werror -Wno-deprecated )

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE} )
set( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE} )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
find_package( Qt6 REQUIRED COMPONENTS Core Widgets )

enable_testing()

add_subdirectory( src )
add_subdirectory( test_unit )
