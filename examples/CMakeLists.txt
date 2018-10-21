cmake_minimum_required(VERSION 3.8)

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

MESSAGE("=====================================================")
MESSAGE(" Collecting Executables                              ")
MESSAGE("=====================================================")
FOREACH(subdir ${SUBDIRS})
  MESSAGE("Compiling Module ${subdir} into executable" )
  add_subdirectory(${subdir})
ENDFOREACH()
MESSAGE("=====================================================")
