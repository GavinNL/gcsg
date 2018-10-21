cmake_minimum_required(VERSION 3.8)

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

MESSAGE("=====================================================")
MESSAGE(" Collecting Modules                                  ")
MESSAGE("=====================================================")
FOREACH(subdir ${SUBDIRS})
  MESSAGE("Compiling Module ${subdir} into lib${subdir}" )
  add_subdirectory(${subdir})
ENDFOREACH()
MESSAGE("=====================================================")

MESSAGE("${CPPBOILERPLATE_LIBS}")
