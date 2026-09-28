#----------- general -----------

cmake_minimum_required(VERSION 3.24)
project(physics)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#----------- dependencies -----------

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# --- devkit:deps ---
find_package(Icore REQUIRED)
find_package(Raylib_impl REQUIRED)
find_package(System REQUIRED)
find_package(GoogleTest REQUIRED)
# --- devkit:end ---

#----------- sources -----------
# everything under sources/ belongs to the object stage, save the two files
# that belong to one output. Drop a .cpp in and it is picked up.

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp)
list(REMOVE_ITEM SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/symbole.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
)

#----------- targets -----------
# The sources are compiled once, in an OBJECT library, and every output
# helps itself to the same .o.
#
# Only what kind asks for is written. To add an output, add it to kind in
# config.yaml: devkit regenerates on the spot.

# --- devkit:targets ---
add_library(${PROJECT_NAME}_objects OBJECT ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME}_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)

set(DEVKIT_OBJECTS ${PROJECT_NAME}_objects)

add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}_objects>)
set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
add_library(${PROJECT_NAME} ALIAS ${PROJECT_NAME}_static)

set(DEVKIT_TARGETS ${PROJECT_NAME}_static)
# --- devkit:end ---

# the object stage compiles the sources: it needs the same includes and the
# same options as the outputs. $<TARGET_OBJECTS:> carries nothing but the .o,
# so the outputs receive them too, for whoever consumes them.
foreach(target IN LISTS DEVKIT_OBJECTS DEVKIT_TARGETS)
  target_include_directories(${target} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
  )

  target_compile_options(${target} PRIVATE -Wall -Wextra)
endforeach()

# --- devkit:link ---
set(DEVKIT_LINK "")
foreach(dep IN ITEMS system)
  if (TARGET ${dep}_static)
    list(APPEND DEVKIT_LINK ${dep}_static)
  else()
    list(APPEND DEVKIT_LINK ${dep})
  endif()
endforeach()

foreach(target IN LISTS DEVKIT_OBJECTS DEVKIT_TARGETS)
  target_link_libraries(${target} PUBLIC ${DEVKIT_LINK})
endforeach()
# --- devkit:end ---

# --- devkit:shared ---
# --- devkit:end ---

# --- devkit:example ---
#----------- examples -----------
#one folder each under examples/, with their own main, so that none
#of them crosses the product's

set(DEVKIT_EXAMPLE_LINK "")
foreach(dep IN ITEMS icore raylib_impl)
  if (TARGET ${dep}_static)
    list(APPEND DEVKIT_EXAMPLE_LINK ${dep}_static)
  else()
    list(APPEND DEVKIT_EXAMPLE_LINK ${dep})
  endif()
endforeach()

add_executable(${PROJECT_NAME}_basic examples/basic/main.cpp)
target_include_directories(${PROJECT_NAME}_basic PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/includes
  ${CMAKE_CURRENT_SOURCE_DIR}/examples/basic
)
target_link_libraries(${PROJECT_NAME}_basic PRIVATE ${DEVKIT_EXAMPLE_LINK})
if (TARGET ${PROJECT_NAME}_static)
  target_link_libraries(${PROJECT_NAME}_basic PRIVATE ${PROJECT_NAME}_static)
endif()
# --- devkit:end ---

# --- devkit:tests ---
#----------- tests -----------

enable_testing()

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)

#without this test, an empty tests/ folder would fail add_executable
if (TEST_SOURCES)
  add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES} $<TARGET_OBJECTS:${PROJECT_NAME}_objects>)

  target_include_directories(${PROJECT_NAME}_tests PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
  )

  target_link_libraries(${PROJECT_NAME}_tests gtest_main ${DEVKIT_LINK})

  include(GoogleTest)
  gtest_discover_tests(${PROJECT_NAME}_tests)
endif()
# --- devkit:end ---
