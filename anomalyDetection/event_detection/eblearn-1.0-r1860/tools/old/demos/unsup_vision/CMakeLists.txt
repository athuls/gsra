################################################################################
#
# CMake configuration for the unsup_vision demo project
#
# Author(s):
#   Pierre Sermanet, pierre.sermanet@gmail.com, New York University
#
################################################################################

# add include directories
##############################################################################
include_directories(include)
include_directories(${LIBEBLEARN_INCLUDE_DIR})
include_directories(${LIBIDX_INCLUDE_DIR})
include_directories(${EBLEARN_TOOLS_INCLUDE_DIR})

FIND_PACKAGE(Qt4)
IF (QT_FOUND) #-----------------------------------------------------------------
  include_directories(${LIBIDXGUI_INCLUDE_DIR})
  include_directories(${LIBEBLEARNGUI_INCLUDE_DIR})
  include_directories(${QT_INCLUDE_DIR})
  include_directories(${QT_QTGUI_INCLUDE_DIR})
  include_directories(${QT_QTCORE_INCLUDE_DIR})
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__GUI__")
ENDIF (QT_FOUND) #--------------------------------------------------------------

# compile executable
################################################################################
add_executable (unsup_vision
  src/main.cpp
  )
  
# link executable with external libraries
################################################################################
target_link_libraries (unsup_vision eblearn idx eblearntools)  
IF (QT_FOUND) #---------------------------------------------------------------
  target_link_libraries (unsup_vision idxgui)
  target_link_libraries (unsup_vision eblearngui)
ENDIF (QT_FOUND) #------------------------------------------------------------
