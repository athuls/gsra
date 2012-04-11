################################################################################
#
# CMake configuration for the posture demo project
#
# Author(s):
#   Clement farabet, clement.farabet@gmail.com, New York University
#
################################################################################

include_directories(include)
include_directories(${LIBEBLEARN_INCLUDE_DIR})
include_directories(${LIBIDX_INCLUDE_DIR})
include_directories(${EBLEARN_TOOLS_INCLUDE_DIR})


FIND_PACKAGE(Qt4)
IF (QT_FOUND) #-----------------------------------------------------------------
  # add include directories
  ##############################################################################
  include_directories(${LIBIDXGUI_INCLUDE_DIR})
  include_directories(${LIBEBLEARNGUI_INCLUDE_DIR})
  include_directories(${QT_INCLUDE_DIR})
  include_directories(${QT_QTGUI_INCLUDE_DIR})
  include_directories(${QT_QTCORE_INCLUDE_DIR})
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__GUI__")
ENDIF (QT_FOUND) #--------------------------------------------------------------


# compile executable
################################################################################
add_executable (posture
  src/main.cpp
  )
  
# link executable with external libraries
################################################################################
target_link_libraries (posture eblearn idx)
  
  IF (QT_FOUND) #---------------------------------------------------------------
    target_link_libraries (posture idxgui)
    target_link_libraries (posture eblearngui)
  ENDIF (QT_FOUND) #------------------------------------------------------------
  