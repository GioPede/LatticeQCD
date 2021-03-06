# Specify the minimum version for CMake

cmake_minimum_required(VERSION 2.8)

# Project's name
project(LatticeQCD)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_CACHEFILE_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

set(QMC_MPI 1)
set(CMAKE_CXX_COMPILER mpic++)

# The following folder will be included
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add all src
set(SHARED_SRC ${PROJECT_SOURCE_DIR}/ParallelTools/parallel.cpp
               ${PROJECT_SOURCE_DIR}/Math/su3.cpp 
               ${PROJECT_SOURCE_DIR}/Math/point.cpp 
               ${PROJECT_SOURCE_DIR}/Math/random.cpp 
               ${PROJECT_SOURCE_DIR}/Math/lattice.cpp 
               ${PROJECT_SOURCE_DIR}/Math/complex.cpp 
               ${PROJECT_SOURCE_DIR}/Actions/action.cpp
               ${PROJECT_SOURCE_DIR}/Actions/puregauge.cpp
               ${PROJECT_SOURCE_DIR}/Observables/observable.cpp
               ${PROJECT_SOURCE_DIR}/Observables/plaquette.cpp
               ${PROJECT_SOURCE_DIR}/Observables/topologicalcharge.cpp
               ${PROJECT_SOURCE_DIR}/InputOutput/inputparser.cpp
               ${PROJECT_SOURCE_DIR}/InputOutput/inputconf.cpp
               ${PROJECT_SOURCE_DIR}/InputOutput/outputconf.cpp
               ${PROJECT_SOURCE_DIR}/InputOutput/outputobs.cpp
               ${PROJECT_SOURCE_DIR}/InputOutput/outputterm.cpp
               ${PROJECT_SOURCE_DIR}/Apps/app.cpp
               ${PROJECT_SOURCE_DIR}/Apps/gaugefieldfactory.cpp
               ${PROJECT_SOURCE_DIR}/Apps/gaugefieldreader.cpp
)

add_library( sharedResources ${SHARED_SRC} )

# Set executables
add_executable(pgfg ${PROJECT_SOURCE_DIR}/MainPrograms/pgfg.cpp)
add_executable(gfr ${PROJECT_SOURCE_DIR}/MainPrograms/gfr.cpp)
#add_executable(gfe ${PROJECT_SOURCE_DIR}/MainPrograms/gfe.cpp)

target_link_libraries( pgfg sharedResources )
target_link_libraries( gfr sharedResources )
#target_link_libraries( gfe sharedResources )

# Set compiler and flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")


