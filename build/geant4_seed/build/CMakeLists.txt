cmake_minimum_required(VERSION 3.16)
project(simple_pp)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

add_executable(simple_pp simple_pp.cc)
target_link_libraries(simple_pp ${Geant4_LIBRARIES})
